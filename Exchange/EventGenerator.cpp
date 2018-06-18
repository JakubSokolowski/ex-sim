#include "EventGenerator.h"
#include "RandUtil.h"


EventGenerator::EventGenerator(CommChannel &ch)
    : channel(ch)
    , event_thread_m(&EventGenerator::GenerateEvents, this)
{}

void EventGenerator::GenerateEvents() {
//    while (channel.exchange_closed)
//        channel.coordinator.Wait();
    channel.coordinator.Wait();
    while(!channel.exchange_closed){
        auto type = static_cast<EventType >(RandomInRange(0,2));
        events_m.PushFront(Event{counter_m, type});
        counter_m++;
        WaitRandom(5000, 5000);
    }
}

Event EventGenerator::CurrentEvent()  {
    return events_m.Front();
}

EventGenerator::~EventGenerator() {
    if(event_thread_m.joinable())
        event_thread_m.join();
}

