//
// Created by jakub on 15.06.18.
//

#ifndef HESOYAM_SAFEQUEUE_H
#define HESOYAM_SAFEQUEUE_H


#include <queue>
#include <mutex>
#include <condition_variable>
#include <boost/lockfree/queue.hpp>

// A threadsafe-queue.
template <class T>
class SafeQueue
{
public:
    SafeQueue()
            : q()
            , m()
            , c()
    {}

    ~SafeQueue() = default;

    // Add an element to the queue.
    void PushBack(T t) {
        std::lock_guard<std::mutex> lock(m);
        q.push_back(t);
        c.notify_one();
    }

    T Front() {
        std::unique_lock<std::mutex> lock(m);
        while(q.empty()) {
            c.wait(lock);
        }
        T val = q.front();
        return val;
    }

    bool IsEmpty() const { return q.empty(); }
    ulong Size() const {return q.size();}

    void PushFront(T t) {
        std::lock_guard<std::mutex> lock(m);
        q.push_front(t);
        c.notify_one();
    }


    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    T PopFront() {
        std::unique_lock<std::mutex> lock(m);
        // release lock as long as the wait and reaquire it afterwards.
        while(q.empty()) {
            c.wait(lock);
        }
        T val = q.front();
        q.pop_front();
        return val;
    }

private:

    std::deque<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};

#endif //HESOYAM_SAFEQUEUE_H
