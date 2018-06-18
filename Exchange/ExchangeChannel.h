#ifndef HESOYAM_EXCHANGECHANNEL_H
#define HESOYAM_EXCHANGECHANNEL_H


#include "Coordinator.h"
#include "MatchingEngine.h"
#include "SafeQueue.h"

class ExchangeChannel {
public:
    ExchangeChannel(CommChannel &channel, MatchingEngine &eng);
    CommChannel &channel_m;
    MatchingEngine &engine;
    SafeQueue<Order> order_queue;
};


#endif //HESOYAM_EXCHANGECHANNEL_H
