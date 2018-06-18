#include "Order.h"
std::atomic<uint> Order::unique_count_m {0};
Order::Order(uint price, uint amount, int player_id, OrderType ot, PlayerType pt)
    : price_m(price)
    , amount_m(amount)
    , player_id_m(player_id)
    , order_type_m(ot)
    , player_type_m(pt)
{
    id_m = unique_count_m;
    unique_count_m++;
}


bool Order::operator==(const Order &other) const {
    return id_m == other.id_m;
}

bool Order::operator!=(const Order &other) const {
    return !(*this == other);
}

void Order::SetId(int id) {
    id_m = id;
}


