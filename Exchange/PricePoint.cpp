#include <stdexcept>
#include "PricePoint.h"

PricePoint::PricePoint(std::deque<Order> &orders) noexcept
: orders_m(orders)
{}
PricePoint::PricePoint(const PricePoint &other) noexcept
    : orders_m(other.orders_m)
    , total_amount_m(other.total_amount_m)
    , price_m(other.price_m)
    , type_m(other.type_m)
{}

PricePoint::PricePoint(PricePoint &&other) noexcept
    : orders_m(std::move(other.orders_m))
    , total_amount_m(other.total_amount_m)
    , price_m(other.price_m)
    , type_m(other.type_m)
{}

PricePoint::PricePoint(Order order) noexcept {
    AddOrder(order);
}


void PricePoint::AddOrder(Order order) {
    if(!price_m) {
        price_m = order.price_m;
        type_m = order.order_type_m;
    }
    if(order.price_m != price_m)
        throw std::invalid_argument("Order price does not match price point");
    if(type_m != order.order_type_m)
        throw std::invalid_argument("Order type does not match order types of this PricePoint");
    if(order.player_type_m == PlayerType::INSIDER)
        orders_m.emplace_front(order);
    else
        orders_m.emplace_back(order);
    AddOrderStats(order);
}

void PricePoint::RemoveOrder(int order_id) {
    auto predicate =  [&order_id] (const Order&o) {return o.id_m == order_id;};
    auto it = std::find_if(orders_m.begin(), orders_m.end(), predicate );
    if(it != orders_m.end()) {
        orders_m.erase(it);
        RemoveOrderStats(*it);
    }

}

void PricePoint::RemovePlayerOrders(int player_id) {
    auto predicate = [&player_id] (const Order& o) {return o.player_id_m == player_id;};
    uint removed_amount = 0;
    auto sum_predicate = [&player_id, &removed_amount](const Order& o) {
        if(player_id == o.player_id_m)
            removed_amount += o.amount_m;
    };
    std::for_each(orders_m.begin(), orders_m.end(), sum_predicate);
    orders_m.erase(std::remove_if(orders_m.begin(), orders_m.end(), predicate), orders_m.end());
    total_amount_m -= removed_amount;
}

Order PricePoint::PopOrder() {
    auto order = orders_m.front();
    orders_m.pop_front();
    RemoveOrderStats(order);
    return order;
}

Order PricePoint::GetFirst() const {
    return orders_m.front();
}

ulong PricePoint::GetOrderCount() const {
    return orders_m.size();
}

bool PricePoint::IsEmpty() const {
    return orders_m.empty();
}

uint PricePoint::GetTotalAmount() const {
    return total_amount_m;
}

uint PricePoint::GetPrice() const {
    return price_m;
}

uint PricePoint::GetTotalPrice() const {
    return price_m * total_amount_m;
}

void PricePoint::RemoveOrderStats(const Order &order) {
    total_amount_m -= order.amount_m;
}

void PricePoint::AddOrderStats(const Order &order) {
    total_amount_m += order.amount_m;
}

void PricePoint::AddOrders(std::deque<Order> &orders) {
    for(auto order: orders)
        AddOrder(order);
}

OrderType PricePoint::GetOrderType() const {
    return type_m;
}


