#ifndef HESOYAM_COORDINATOR_H
#define HESOYAM_COORDINATOR_H


#include <mutex>
#include <condition_variable>
#include <atomic>

class Coordinator {
public:
    void Wait();
    void NotifyAll();
    void NotifyOne();

private:
    std::mutex mutex;
    std::condition_variable cv;
};

struct CommChannel {
    Coordinator coordinator;
    std::atomic<bool> exchange_closed{false};
};


#endif //HESOYAM_COORDINATOR_H
