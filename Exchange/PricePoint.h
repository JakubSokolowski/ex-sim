
#ifndef HESOYAM_PRICEPOINT_H
#define HESOYAM_PRICEPOINT_H

#include "Order.h"
#include <deque>
#include <algorithm>
class PricePoint {
public:
    PricePoint() = default;
    explicit PricePoint(Order order) noexcept ;
    explicit PricePoint(std::deque<Order>& orders) noexcept ;
    PricePoint(const PricePoint& other) noexcept ;
    PricePoint(PricePoint&& other) noexcept ;

    void AddOrder(Order order);
    void AddOrders(std::deque<Order> &orders);
    void AddOrderStats(const Order &order);
    void RemoveOrder(int order_id);
    void RemovePlayerOrders(int player_id);
    void RemoveOrderStats(const Order &order);
    Order PopOrder();

    bool IsEmpty() const;
    Order GetFirst() const;
    ulong GetOrderCount() const;
    uint GetTotalAmount() const;
    uint GetPrice() const;
    uint GetTotalPrice()const;
    OrderType GetOrderType() const;

    std::deque<Order> orders_m;
    uint total_amount_m = 0;
    uint price_m = 0;
    OrderType type_m = OrderType::ASK;
};


#endif //HESOYAM_PRICEPOINT_H
