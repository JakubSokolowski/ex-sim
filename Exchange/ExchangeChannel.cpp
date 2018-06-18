//
// Created by jakub on 16.06.18.
//

#include "ExchangeChannel.h"

ExchangeChannel::ExchangeChannel(CommChannel &channel, MatchingEngine &eng)
    : channel_m(channel)
    , engine(eng)
{}
