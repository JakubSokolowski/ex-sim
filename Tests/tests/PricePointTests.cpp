//
// Created by jakub on 14.06.18.
//
#include <gtest/gtest.h>
#include <PricePoint.h>


TEST(OrderTests, OrdersAreEqual) {
    Order order1(10,10,1, BID, NORMAL);
    Order order2(10,10,1, BID ,NORMAL);
    EXPECT_EQ(order1, order2);
}

TEST(PricePointTests, PricePointOrderAdditionWorks) {
    PricePoint point;
    EXPECT_TRUE(point.IsEmpty());
    Order order(10,10,1, BID ,NORMAL);
    point.AddOrder(order);
    EXPECT_EQ(point.GetOrderType(), BID);
    EXPECT_EQ(point.GetOrderCount(), 1);
    EXPECT_EQ(point.total_amount_m, 10);
    EXPECT_EQ(point.GetPrice(), 10);
    EXPECT_EQ(point.GetTotalPrice(), 100);
    EXPECT_EQ(order, point.GetFirst());
}

TEST(PricePointTests, PricePointMultipleOrderAdditionWorks) {
    PricePoint point;
    std::deque<Order> orders = {
            Order(10, 20, 1, BID ,NORMAL),
            Order(10, 10, 1, BID ,NORMAL),
            Order(10, 10, 1, BID ,NORMAL),
            Order(10, 10, 1, BID ,NORMAL),
            Order(10, 10, 1, BID ,NORMAL),
    };
    point.AddOrders(orders);
    EXPECT_EQ(point.GetOrderCount(), 5);
    EXPECT_EQ(point.total_amount_m, 60);
    EXPECT_EQ(point.GetPrice(), 10);
    EXPECT_EQ(point.GetTotalPrice(), 600);
    EXPECT_EQ(Order(10,20,1, BID ,NORMAL), point.GetFirst());
}

TEST(PricePointTests, PricePointNotMatchingPricesThrowsInvalidArgument) {
    try {
        PricePoint point;
        point.AddOrder(Order(10, 20, 1, BID ,NORMAL));
        point.AddOrder(Order(5, 20, 1, BID ,NORMAL));
        FAIL();
    } catch (const std::invalid_argument& ex) {
        ASSERT_STREQ("Order price does not match price point", ex.what());
    }
}

TEST(PricePointTests, PricePointNotMatchingOrderTypeThrowsInvalidArgument) {
    try {
        PricePoint point;
        point.AddOrder(Order(10, 20, 1, BID ,NORMAL));
        point.AddOrder(Order(10, 20, 1, ASK ,NORMAL));
        FAIL();
    } catch (const std::invalid_argument& ex) {
        ASSERT_STREQ("Order type does not match order types of this PricePoint", ex.what());
    }
}

TEST(PricePointTests, PricePointSingleOrderSingleRemovalWorks) {
    PricePoint point;
    EXPECT_TRUE(point.IsEmpty());
    Order order(10,10,1,BID ,NORMAL);
    point.AddOrder(order);
    EXPECT_EQ(point.GetOrderType(), BID);
    EXPECT_EQ(point.GetOrderCount(), 1);
    EXPECT_EQ(point.total_amount_m, 10);
    EXPECT_EQ(point.GetPrice(), 10);
    EXPECT_EQ(point.GetTotalPrice(), 100);
    EXPECT_EQ(order, point.GetFirst());
    point.RemoveOrder(order.id_m);
    EXPECT_EQ(point.GetOrderCount(), 0);
    EXPECT_EQ(point.total_amount_m, 0);
    EXPECT_EQ(point.GetPrice(), 10);
    EXPECT_EQ(point.GetTotalPrice(), 0);
}

TEST(PricePointTests, PricePointMultipleOrdersSingleRemovalWorks) {
    PricePoint point;
    EXPECT_TRUE(point.IsEmpty());
    Order order(10,20,1, BID ,NORMAL);
    order.SetId(20);
    std::deque<Order> orders = {
            Order(10, 10, 1, BID, NORMAL),
            Order(10, 10, 1, BID, NORMAL),
            Order(10, 10, 1, BID, NORMAL),
            Order(10, 10, 1, BID, NORMAL),
            Order(10, 10, 1, BID, NORMAL),
    };

    point.AddOrder(order);
    point.AddOrders(orders);
    EXPECT_EQ(point.GetOrderType(), BID);
    EXPECT_EQ(point.GetOrderCount(), 6);
    EXPECT_EQ(point.total_amount_m, 70);
    EXPECT_EQ(point.GetTotalPrice(), 700);
    EXPECT_EQ(order, point.GetFirst());
    point.RemoveOrder(order.id_m);
    EXPECT_EQ(point.GetOrderCount(), 5);
    EXPECT_EQ(point.total_amount_m, 50);
    EXPECT_EQ(point.GetTotalPrice(), 500);
    EXPECT_NE(order, orders.front());
}

TEST(PricePointTests, PricePointNonexsistingOrderRemovalFails) {
    PricePoint point;
    Order order(10,2,3,BID, NORMAL);
    point.AddOrder(order);
    EXPECT_EQ(point.GetOrderCount(), 1);
    point.RemoveOrder(order.id_m + 1);
    EXPECT_EQ(point.GetOrderCount(), 1);
}

TEST(PricePointTests, PricePointAllPlayerOrdersRemovalWorks) {
    PricePoint point;
    std::deque<Order> player1_orders = {
            Order(10, 10, 1, BID, NORMAL),
            Order(10, 10, 1, BID, NORMAL),
            Order(10, 10, 1, BID, NORMAL),
            Order(10, 10, 1, BID, NORMAL),
            Order(10, 10, 1, BID, NORMAL),
    };
    std::deque<Order> player2_orders = {
            Order(10, 20, 2, BID, NORMAL),
            Order(10, 20, 2, BID, NORMAL),
            Order(10, 20, 2, BID, NORMAL),
            Order(10, 20, 2, BID, NORMAL),
            Order(10, 20, 2, BID, NORMAL),
    };
    point.AddOrders(player1_orders);
    point.AddOrders(player2_orders);
    EXPECT_EQ(point.GetOrderCount(), 10);
    EXPECT_EQ(point.GetTotalAmount(), 150);
    point.RemovePlayerOrders(2);
    EXPECT_EQ(point.GetOrderCount(), 5);
    EXPECT_EQ(point.GetTotalAmount(), 50);
    EXPECT_EQ(Order(10,10,1, BID, NORMAL), point.GetFirst());
}