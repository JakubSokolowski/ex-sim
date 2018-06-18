#ifndef HESOYAM_ORDERBOOK_H
#define HESOYAM_ORDERBOOK_H

#include <map>
#include <iostream>
#include <deque>
#include "PricePoint.h"
using uint = unsigned int;

// List of buyers and sellers
class OrderBook {
    struct BidAsk{
        uint best_bid, bid_amount, best_ask, ask_amount;
        int spread;
    };
    // bids - orders bying, asks - orders selling
    std::map<uint, PricePoint> bids, asks;
public:
    bool IsEmpty() const;
    void AddOrder(Order order);
    void AddOrders(std::deque<Order> orders);
    void RemoveOrder(Order order);
    void RemoveOrders(std::deque<Order>const &orders);

    PricePoint GetBidPricePoint(uint price);
    PricePoint GetAskPricePoint(uint price);
    uint GetPricePointTotalAmount(uint price, OrderType type);
    ulong GetPricePointOrderCount(uint price, OrderType type);
    uint GetTotalAmount(OrderType type);
    uint GetDepth(OrderType type);
    uint GetOrderCount(OrderType type);

    BidAsk GetBidAsk();

private:

    void AddAsk(Order order);
    void AddBid(Order order);
    void RemoveAsk(Order order);
    void RemoveBid(Order order);


    friend std::ostream& operator<<(std::ostream& os, const OrderBook& book);

};


#endif //HESOYAM_ORDERBOOK_H
