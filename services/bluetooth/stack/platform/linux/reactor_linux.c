/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "platform/include/reactor.h"
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include "platform/include/mutex.h"
#include "platform/include/list.h"
#include "platform/include/semaphore.h"
#include "platform/include/platform_def.h"

typedef struct Reactor {
    int epollFd;
    int stopFd;
    bool isRunning;
    pthread_t threadId;
    List *movedItems;
    // Items whose destruction was deferred by ReactorUnregister. Freed only by the dispatch
    // loop's drain (or by ReactorDelete after the loop has exited). Guarded by apiMutex.
    List *pendingFree;
    Mutex *apiMutex;
} ReactorInternal;

typedef struct ReactorItem {
    int fd;
    Mutex *lock;
    Reactor *reactor;
    void *context;
    void (*onReadReady)(void *context);
    void (*onWriteReady)(void *context);
    // Set by ReactorUnregister while holding the item's lock; the dispatch loop checks it
    // under the same lock before dispatching, so the check and the callback are atomic with
    // respect to any unregister (in-flight callback barrier: once ReactorUnregister returns,
    // no callback for this item can be started or still be running, and the caller may
    // release the context). The item is guaranteed to be alive whenever the flag is read:
    // destruction only happens in the dispatch loop's pendingFree drain or in
    // ReactorUnregister's own !isRunning path while holding the reactor's apiMutex (which no
    // concurrent unregister can be in).
    bool unregistered;
    // Number of ReactorUnregister calls in flight for this item (atomic). The increment and
    // the pendingFree drain's check-and-free are both serialized by the reactor's apiMutex,
    // so an item is never freed between a concurrent unregister's increment and its queueing.
    unsigned int refCount;
} ReactorItemInternal;

const int MAXEPOLLEVENTS = 64;

static inline bool DataCmp(void *data1, void *data2)
{
    if (data1 != data2) {
        return false;
    }
    return true;
}

// Free all items queued for deferred destruction. Called with apiMutex held (dispatch loop
// and its exit paths) or with no concurrency (ReactorDelete after the loop has exited).
// Items with a ReactorUnregister still in flight (refCount > 0) are left queued for the next
// drain round.
static void ReactorDrainPendingFree(Reactor *reactor)
{
    ListNode *node = ListGetFirstNode(reactor->pendingFree);
    while (node != NULL) {
        ListNode *next = ListGetNextNode(node);
        ReactorItem *item = (ReactorItem *)ListGetNodeData(node);
        if (__atomic_load_n(&item->refCount, __ATOMIC_ACQUIRE) > 0) {
            node = next;
            continue;
        }
        ListRemoveNode(reactor->pendingFree, item);
        MutexDelete(item->lock);
        free(item);
        node = next;
    }
}

void ReactorSetThreadId(Reactor *reactor, unsigned long threadId)
{
    reactor->threadId = (pthread_t)threadId;
}

Reactor *ReactorCreate()
{
    Reactor *reactor = (Reactor *)calloc(1, sizeof(Reactor));
    if (reactor == NULL) {
        LOG_ERROR("ReactorCreate: calloc Reactor failed.");
        return NULL;
    }
    reactor->epollFd = -1;
    reactor->stopFd = -1;

    int epollFd = epoll_create1(EPOLL_CLOEXEC);
    if (epollFd == -1) {
        LOG_ERROR("ReatorCreate: epoll create failed, error no: %{public}d.", errno);
        goto ERROR;
    }

    int stopFd = eventfd(0, 0);
    if (stopFd == -1) {
        LOG_ERROR("ReatorCreate: eventfd failed, error no: %{public}d.", errno);
        goto ERROR;
    }

    struct epoll_event event = {0};
    event.data.ptr = NULL;
    event.events = EPOLLIN;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, stopFd, &event) == -1) {
        LOG_ERROR("ReatorCreate: epoll_ctl ADD-Option failed, error no: %{public}d.", errno);
        goto ERROR;
    }

    reactor->movedItems = ListCreate(NULL);
    if (reactor->movedItems == NULL) {
        goto ERROR;
    }
    reactor->pendingFree = ListCreate(NULL);
    if (reactor->pendingFree == NULL) {
        goto ERROR;
    }
    reactor->apiMutex = MutexCreate();
    if (reactor->apiMutex == NULL) {
        goto ERROR;
    }
    reactor->epollFd = epollFd;
    reactor->stopFd = stopFd;

    return reactor;

ERROR:
    ReactorDelete(reactor);
    return NULL;
}

void ReactorDelete(Reactor *reactor)
{
    if (reactor == NULL) {
        return;
    }

    // Free items whose destruction was deferred to the dispatch loop but never drained
    // (the loop exited before getting to them). Per contract no unregister is in flight
    // here, so no item can be deferred by a nonzero refCount.
    if (reactor->pendingFree != NULL) {
        ReactorDrainPendingFree(reactor);
        ListDelete(reactor->pendingFree);
    }
    MutexDelete(reactor->apiMutex);
    ListDelete(reactor->movedItems);
    close(reactor->stopFd);
    close(reactor->epollFd);
    free(reactor);
}

// Dispatches one collected epoll event: returns true when the stopFd was signaled (the
// dispatch loop must exit), false otherwise. A moved item and an item unregistered after
// this event was collected are skipped like the original loop's continue paths.
static bool ReactorDispatchEvent(Reactor *reactor, struct epoll_event *event)
{
    if (event->data.ptr == NULL) {
        eventfd_t val;
        eventfd_read(reactor->stopFd, &val);
        reactor->isRunning = false;
        MutexLock(reactor->apiMutex);
        ReactorDrainPendingFree(reactor);
        MutexUnlock(reactor->apiMutex);
        return true;
    }

    ReactorItem *item = (ReactorItem *)event->data.ptr;

    MutexLock(reactor->apiMutex);
    if (ListForEachData(reactor->movedItems, DataCmp, item) != NULL) {
        MutexUnlock(reactor->apiMutex);
        return false;
    }

    // The unregistered check runs under the item lock, the same lock ReactorUnregister
    // holds while setting the flag (the in-flight callback barrier): the check and the
    // dispatch are atomic with respect to any unregister. Once an unregister has
    // returned, no callback for this item can be started or still be running.
    MutexLock(item->lock);
    if (item->unregistered) {
        // Unregistered after this event was collected: the item is still alive (the
        // pendingFree drain, the only freeer, last ran before this batch was
        // collected), so skipping the stale event here is safe.
        MutexUnlock(item->lock);
        MutexUnlock(reactor->apiMutex);
        return false;
    }
    MutexUnlock(reactor->apiMutex);
    if ((event->events & (EPOLLIN | EPOLLRDHUP)) && (item->onReadReady != NULL)) {
        item->onReadReady(item->context);
    }
    // A concurrent unregister cannot set the flag while the item lock is held (it needs
    // the lock for the barrier), and an unregister completed before we took the lock
    // was already seen by the check above; the re-read below is kept as a defensive
    // guard for symmetry with the onReadReady path.
    if ((event->events & EPOLLOUT) && (item->onWriteReady != NULL) && (!item->unregistered)) {
        item->onWriteReady(item->context);
    }
    MutexUnlock(item->lock);
    return false;
}

int32_t ReactorStart(Reactor *reactor)
{
    ASSERT(reactor);

    reactor->isRunning = true;

    struct epoll_event events[MAXEPOLLEVENTS];
    for (;;) {
        MutexLock(reactor->apiMutex);
        // Free items unregistered in earlier rounds. This runs before epoll_wait, so an
        // item destroyed here can never be referenced by the batch collected afterwards:
        // ReactorUnregister removes the fd from epoll before queuing the item.
        ReactorDrainPendingFree(reactor);
        ListClear(reactor->movedItems);
        MutexUnlock(reactor->apiMutex);

        int nfds;
        CHECK_EXCEPT_INTR(nfds = epoll_wait(reactor->epollFd, events, MAXEPOLLEVENTS, -1));
        if (nfds == -1) {
            reactor->isRunning = false;
            LOG_ERROR("ReactorStart: epoll_wait failed, error no: %{public}d.", errno);
            MutexLock(reactor->apiMutex);
            ReactorDrainPendingFree(reactor);
            MutexUnlock(reactor->apiMutex);
            return -1;
        }

        for (int i = 0; i < nfds; ++i) {
            if (ReactorDispatchEvent(reactor, &events[i])) {
                return 0;
            }
        }
    }
}

void ReactorStop(const Reactor *reactor)
{
    ASSERT(reactor);
    eventfd_write(reactor->stopFd, 1);
}

ReactorItem *ReactorRegister(
    Reactor *reactor, int fd, void *context, void (*onReadReady)(void *context), void (*onWriteReady)(void *context))
{
    ASSERT(reactor);

    ReactorItem *item = (ReactorItem *)calloc(1, (sizeof(ReactorItem)));

    item->lock = MutexCreate();
    if (item->lock == NULL) {
        goto ERROR;
    }

    item->fd = fd;
    item->context = context;
    item->reactor = reactor;
    item->onReadReady = onReadReady;
    item->onWriteReady = onWriteReady;

    struct epoll_event event = {0};
    event.data.ptr = item;
    if (onReadReady != NULL) {
        event.events |= (EPOLLIN | EPOLLRDHUP);
    }
    if (onWriteReady != NULL) {
        event.events |= EPOLLOUT;
    }

    if (epoll_ctl(reactor->epollFd, EPOLL_CTL_ADD, item->fd, &event) == -1) {
        goto ERROR;
    }

    return item;

ERROR:
    if (item != NULL) {
        MutexDelete(item->lock);
        free(item);
    }

    return NULL;
}

void ReactorUnregister(ReactorItem *item)
{
    ASSERT(item);

    MutexLock(item->reactor->apiMutex);

    // Take the in-flight reference under apiMutex: the increment and the pendingFree drain's
    // check-and-free are both serialized by apiMutex, so the item is never freed between a
    // concurrent unregister's increment and its queueing, and the item stays alive for the
    // whole unregister even if a concurrent unregister has already queued it.
    __atomic_fetch_add(&item->refCount, 1, __ATOMIC_RELEASE);

    struct epoll_event event = {0};
    if (epoll_ctl(item->reactor->epollFd, EPOLL_CTL_DEL, item->fd, &event) != 0) {
        LOG_ERROR("ReactorUnregister: epoll_ctl delete-option failed, error no: %{public}d.", errno);
    }

    // In-flight callback barrier: check and set the flag under the item lock, the same lock
    // the dispatch loop holds while checking the flag and running the callback. Once this
    // returns, no dispatch can start the callback anymore and any callback that was running
    // has completed, so the caller (e.g. AlarmDelete) may release the context.
    MutexLock(item->lock);
    if (item->unregistered) {
        // A concurrent unregister already queued this item; destruction is owned by the
        // drain (or by the !isRunning path below), just release the reference.
        MutexUnlock(item->lock);
        __atomic_fetch_sub(&item->refCount, 1, __ATOMIC_RELAXED);
        MutexUnlock(item->reactor->apiMutex);
        return;
    }
    item->unregistered = true;
    MutexUnlock(item->lock);

    if (!item->reactor->isRunning) {
        // The dispatch loop has exited (isRunning is written without the lock right before
        // the loop returns, so once false the loop never touches any item again). Holding the
        // apiMutex guarantees no concurrent unregister is in flight, so the reference is the
        // sole one and this item is not queued for deferred free (that would mean unregistered
        // was already set): destroy it directly.
        __atomic_fetch_sub(&item->refCount, 1, __ATOMIC_RELAXED);
        MutexUnlock(item->reactor->apiMutex);
        MutexDelete(item->lock);
        free(item);
        return;
    }
    // The loop is running: defer destruction to its pendingFree drain. The loop is the only
    // freeer, so an item is never destroyed while an event batch may still reference it, and
    // an epoll entry for a freed item can never be collected (the fd is removed first).
    ListAddLast(item->reactor->pendingFree, item);
    __atomic_fetch_sub(&item->refCount, 1, __ATOMIC_RELAXED);
    MutexUnlock(item->reactor->apiMutex);
}
