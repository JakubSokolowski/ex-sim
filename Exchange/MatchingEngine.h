#ifndef HESOYAM_MATCHINGENGINE_H
#define HESOYAM_MATCHINGENGINE_H

#include "OrderBook.h"
struct Payout {
    int to_id = -1;
    uint stock_amount = 0;
    uint funds = 0;
    int order_id = -1;
    Payout(int to, uint amount, uint money, int order)
        : to_id(to)
        , stock_amount(amount)
        , funds(money)
        , order_id(order)
    {}
};

class MatchingEngine {
public:
    MatchingEngine();
    std::deque<Payout> MatchOrder(Order order);
    bool CanBeFilled(Order order);
    OrderBook book;
    ulong volume = 0;
    int current_stock_price_m = 0;

private:
    void AddOrder(Order order);
    std::deque<Payout> FillOrder(Order order);
    std::deque<Payout> FillAsk(Order order);
    std::deque<Payout> FillBid(Order order);
    int unique_count_m = 0;
};


#endif //HESOYAM_MATCHINGENGINE_H
