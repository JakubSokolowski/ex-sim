#include "OrderBook.h"

bool OrderBook::IsEmpty() const {
    return bids.empty() && asks.empty();
}

void OrderBook::AddOrder(Order order) {
    switch(order.order_type_m) {
        case ASK:
            AddAsk(order);
            break;
        case BID:
            AddBid(order);
            break;
    }
}


void OrderBook::RemoveOrder(Order order) {
    switch(order.order_type_m) {
        case OrderType::BID:
            RemoveBid(order);
            break;
        case OrderType::ASK:
            RemoveAsk(order);
            break;
    }
}


void OrderBook::AddAsk(Order order) {
    auto it = asks.lower_bound(order.price_m);
    if(it != asks.end() && ! (asks.key_comp()(order.price_m, it->first))){
        asks[order.price_m].AddOrder(order);
    }
    asks.insert(it, std::make_pair(order.price_m, PricePoint(order)));
}

void OrderBook::AddBid(Order order) {
    auto it = bids.lower_bound(order.price_m);
    if(it != bids.end() && ! (bids.key_comp()(order.price_m, it->first))){
        bids[order.price_m].AddOrder(order);
    }
    bids.insert(it, std::make_pair(order.price_m, PricePoint(order)));
}

void OrderBook::RemoveAsk(Order order) {
    auto it = asks.lower_bound(order.price_m);
    if(it != asks.end() && ! (asks.key_comp()(order.price_m, it->first))){
        asks[order.price_m].RemoveOrder(order.id_m);
    }
}

void OrderBook::RemoveBid(Order order) {
    auto it = bids.lower_bound(order.price_m);
    if(it != bids.end() && ! (bids.key_comp()(order.price_m, it->first))){
        bids[order.price_m].RemoveOrder(order.id_m);
    }
}


PricePoint OrderBook::GetBidPricePoint(uint price) {
    return bids[price];
}

PricePoint OrderBook::GetAskPricePoint(uint price) {
    return asks[price];
}

void OrderBook::RemoveOrders(std::deque<Order>const &orders) {
    for(auto const order : orders)
        RemoveOrder(order);
}

uint OrderBook::GetPricePointTotalAmount(uint price, OrderType type) {
    auto &book = type == OrderType:: ASK ? asks : bids;
    return book[price].GetTotalAmount();
}

ulong OrderBook::GetPricePointOrderCount(uint price, OrderType type) {
    auto &book = type == OrderType:: ASK ? asks : bids;
    return book[price].GetOrderCount();
}

uint OrderBook::GetTotalAmount(OrderType type) {
    auto &book = type == OrderType:: ASK ? asks : bids;
    uint total_amount = 0;
    for(auto point : book)
        total_amount += point.second.GetTotalAmount();
    return total_amount;
}

uint OrderBook::GetDepth(OrderType type) {
    auto &book = type == OrderType:: ASK ? asks : bids;
    uint total_depth = 0;
    for(auto point : book)
        total_depth += point.second.GetTotalPrice();
    return total_depth;
}

uint OrderBook::GetOrderCount(OrderType type) {
    auto &book = type == OrderType:: ASK ? asks : bids;
    uint order_count = 0;
    for(auto point : book)
        order_count += point.second.GetOrderCount();
    return order_count;
}

OrderBook::BidAsk OrderBook::GetBidAsk() {
    uint bid, bid_amount, ask, ask_amount;
    int spread = -1;
    if(bids.rbegin() != bids.rend()) {
        bid = bids.rbegin()->second.GetFirst().price_m;
        bid_amount = bids.rbegin()->second.GetTotalAmount();
    }
    else{
        bid = 0;
        bid_amount = 0;
    }
    if(asks.begin() != asks.end()) {
        ask = asks.begin()->second.GetFirst().price_m;
        ask_amount = asks.begin()->second.GetTotalAmount();
        spread = ask - bid;
    }
    else {
        ask = 0;
        ask_amount = 0;
    }
    return OrderBook::BidAsk{bid, bid_amount, ask, ask_amount, spread};
}

void OrderBook::AddOrders(std::deque<Order> orders) {
    for(auto order : orders)
        AddOrder(order);
}
