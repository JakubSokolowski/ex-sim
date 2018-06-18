#ifndef HESOYAM_EVENTGENERATOR_H
#define HESOYAM_EVENTGENERATOR_H

#include <thread>
#include "SafeQueue.h"
#include "Coordinator.h"

enum EventType {
    GOOD,
    BAD,
    NEUTRAL,
};

struct Event {
    int id;
    EventType type;
};

class EventGenerator {
public:
    explicit EventGenerator(CommChannel &channel);
    ~EventGenerator();
    Event CurrentEvent();

private:
    void GenerateEvents();

    CommChannel &channel;
    std::thread event_thread_m;
    SafeQueue<Event> events_m;
    int counter_m = 0;
};


#endif //HESOYAM_EVENTGENERATOR_H
