//
// Created by jakub on 14.06.18.
//

#ifndef HESOYAM_ORDER_H
#define HESOYAM_ORDER_H

#include <atomic>

enum OrderType {
    BID,
    ASK
};

enum OrderState {
    UNFILLED,
    PARTIALLY_FILLED,
    FILLED,
};

enum PlayerType {
    NORMAL,
    INSIDER
};

using uint = unsigned int;
using ulong = unsigned long;
class Order {
public:

    Order(uint price, uint amount, int player_id, OrderType ot, PlayerType pt);
    Order(const Order& other) = default;
    void SetId(int id);

    uint TotalSum() {return price_m * amount_m; }
    bool operator ==(const Order& other) const;
    bool operator !=(const Order& other) const;

    uint price_m = 0;
    uint amount_m = 0;
    static std::atomic<uint> unique_count_m;
    int id_m = -1;
    int player_id_m = -1;
    OrderType order_type_m = OrderType::ASK;
    PlayerType player_type_m = PlayerType::NORMAL;
    OrderState order_state_m = OrderState::UNFILLED;

};


#endif //HESOYAM_ORDER_H
