//
// Created by jakub on 15.06.18.
//

#include "MatchingEngine.h"

void MatchingEngine::AddOrder(Order order) {
    book.AddOrder(order);
}

MatchingEngine::MatchingEngine() {

}

std::deque<Payout> MatchingEngine::MatchOrder(Order order) {
    std::deque<Payout> payout;
    if(CanBeFilled(order))
        payout = FillOrder(order);
    else
        AddOrder(order);
    for(auto pay : payout)
        volume += pay.funds;

    return payout;
}

std::deque<Payout> MatchingEngine::FillOrder(Order order) {
    // Check if order can be filled in full
    std::deque<Payout> payout;
    switch(order.order_type_m) {
        case BID:
            payout = FillBid(order);
            break;
        case ASK:
            payout = FillAsk(order);
            break;
    }
    current_stock_price_m = order.price_m;
    return payout;
}

bool MatchingEngine::CanBeFilled(Order order) {
    auto ba = book.GetBidAsk();
    switch(order.order_type_m) {
        case BID:
            if(order.price_m >= ba.best_ask && ba.best_ask != 0)
                return true;
            break;
        case ASK:
            if(order.price_m <= ba.best_bid)
                return true;
            break;
        default:
            break;
    }
    return false;
}

// Fills Buying order. Returns deque with the stock payout for bid order
// and cash payouts for filled ask
// orders
std::deque<Payout> MatchingEngine::FillBid(Order order){
    if(order.order_type_m != BID)
        throw std::invalid_argument("For FillAsk the order type must be ASK");
    std::deque<Order> to_remove;
    std::deque<Payout> payout;
    uint amount_filled = 0;
    order.order_state_m = PARTIALLY_FILLED;
    for(auto &ask : book.GetAskPricePoint(order.price_m).orders_m) {
        // Ask in price point can be filled in full, order partially filled
        if(order.amount_m > amount_filled + ask.amount_m) {
            ask.order_state_m = FILLED;
            amount_filled += ask.amount_m;
            payout.emplace_back(ask.player_id_m, 0, ask.price_m * ask.amount_m, order.id_m);
            to_remove.push_back(ask);
            continue;
        }
        // Ask in price point partially filled, order fully filled
        if(order.amount_m < amount_filled + ask.amount_m) {
            ask.order_state_m = PARTIALLY_FILLED;
            order.order_state_m = FILLED;
            uint difference = order.amount_m - amount_filled;
            ask.amount_m -= difference;
            amount_filled += difference;
            payout.emplace_back(ask.player_id_m, 0, ask.price_m * difference, order.id_m);
            break;
        }
        // Both ask and order are filled
        if(order.amount_m == amount_filled + ask.amount_m) {
            ask.order_state_m = FILLED;
            order.order_state_m = FILLED;
            amount_filled += ask.amount_m;
            to_remove.push_back(ask);
            payout.emplace_back(ask.player_id_m, 0, ask.price_m * ask.amount_m, order.id_m);
            // payout.emplace_back(ask.player_id_m, 0, ask.price_m * ask.amount_m);
            break;
        }
    }
    if(order.order_state_m == PARTIALLY_FILLED) {
        order.amount_m -= amount_filled;
        book.AddOrder(order);
    }
    payout.emplace_back(order.player_id_m, amount_filled, 0, order.id_m);
    book.RemoveOrders(to_remove);
    return payout;
}

std::deque<Payout> MatchingEngine::FillAsk(Order order){
    if(order.order_type_m != ASK)
        throw std::invalid_argument("For FillAsk the order type must be ASK");
    std::deque<Order> to_remove;
    std::deque<Payout> payout;
    uint amount_filled = 0;
    order.order_state_m = PARTIALLY_FILLED;
    for(auto &bid : book.GetBidPricePoint(order.price_m).orders_m) {
        // Ask in price point can be filled in full, order partially filled
        if(order.amount_m > amount_filled + bid.amount_m) {
            bid.order_state_m = FILLED;
            amount_filled += bid.amount_m;
            payout.emplace_back(bid.player_id_m, bid.amount_m, 0, order.id_m);
            to_remove.push_back(bid);
            continue;
        }
        // Ask in price point partially filled, order fully filled
        if(order.amount_m < amount_filled + bid.amount_m) {
            bid.order_state_m = PARTIALLY_FILLED;
            order.order_state_m = FILLED;
            uint difference = order.amount_m - amount_filled;
            bid.amount_m -= difference;
            amount_filled += difference;
            payout.emplace_back(bid.player_id_m, difference, 0, order.id_m);
//            payout.emplace_back(bid.player_id_m, difference, 0);
            break;
        }
        // Both ask and order are filled
        if(order.amount_m == amount_filled + bid.amount_m) {
            bid.order_state_m = FILLED;
            order.order_state_m = FILLED;
            amount_filled += bid.amount_m;
            to_remove.push_back(bid);
            payout.emplace_back(bid.player_id_m, bid.amount_m, 0, order.id_m);
            break;
        }
    }
    if(order.order_state_m == PARTIALLY_FILLED) {
        order.amount_m -= amount_filled;
        book.AddOrder(order);
    }
    payout.emplace_back(order.player_id_m, 0, amount_filled * order.price_m, order.id_m);
    book.RemoveOrders(to_remove);
    // Calculate volume
    for(auto pay : payout)
        volume += pay.funds;
    return payout;
}
