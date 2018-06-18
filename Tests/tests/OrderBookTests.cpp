#include <gtest/gtest.h>
#include <OrderBook.h>

TEST(OrderBookTests, OrderBookEmptyByDefault) {
    OrderBook book;
    EXPECT_TRUE(book.IsEmpty());
}
TEST(OrderBookTests, OrderAdditionWorks) {
    OrderBook book;

    book.AddOrder(Order(21, 10, 2, ASK, NORMAL));
    book.AddOrder(Order(21, 10, 2, ASK, NORMAL));
    book.AddOrder(Order(21, 10, 2, ASK, NORMAL));
    book.AddOrder(Order(20, 10, 2, ASK, NORMAL));
    book.AddOrder(Order(20, 10, 2, ASK, NORMAL));
    book.AddOrder(Order(15, 10, 2, ASK, NORMAL));

    book.AddOrder(Order(14, 10, 2, BID, NORMAL));
    book.AddOrder(Order(14, 10, 2, BID, NORMAL));
    book.AddOrder(Order(14, 10, 2, BID, NORMAL));
    book.AddOrder(Order(12, 10, 2, BID, NORMAL));
    book.AddOrder(Order(12, 10, 2, BID, NORMAL));
    book.AddOrder(Order(10, 20, 2, BID, NORMAL));

    EXPECT_EQ(book.GetPricePointOrderCount(21, ASK), 3);
    EXPECT_EQ(book.GetPricePointOrderCount(12, BID), 2);
    EXPECT_EQ(book.GetOrderCount(ASK), 6);
    EXPECT_EQ(book.GetOrderCount(BID), 6);
    EXPECT_EQ(book.GetTotalAmount(ASK), 60);
    EXPECT_EQ(book.GetTotalAmount(BID), 70);
    EXPECT_EQ(book.GetDepth(ASK), 1180);
    EXPECT_EQ(book.GetDepth(BID), 860);
}

TEST(OrderBookTests, OrderRemovalWorks) {
    OrderBook book;

    Order order1(21,10,2,ASK, NORMAL);
    Order order2(11,10,2,BID, NORMAL);
    book.AddOrder(Order(21, 10, 2, ASK, NORMAL));
    book.AddOrder(Order(21, 10, 2, ASK, NORMAL));
    book.AddOrder(Order(21, 10, 2, ASK, NORMAL));
    book.AddOrder(Order(20, 10, 2, ASK, NORMAL));
    book.AddOrder(Order(20, 10, 2, ASK, NORMAL));
    book.AddOrder(Order(15, 10, 2, ASK, NORMAL));

    book.AddOrder(Order(14, 10, 2, BID, NORMAL));
    book.AddOrder(Order(14, 10, 2, BID, NORMAL));
    book.AddOrder(Order(14, 10, 2, BID, NORMAL));
    book.AddOrder(Order(12, 10, 2, BID, NORMAL));
    book.AddOrder(Order(12, 10, 2, BID, NORMAL));
    book.AddOrder(Order(10, 20, 2, BID, NORMAL));

    book.AddOrder(order1);
    book.AddOrder(order2);

    EXPECT_EQ(book.GetPricePointOrderCount(21, ASK), 4);
    EXPECT_EQ(book.GetPricePointOrderCount(11, BID), 1);
    EXPECT_EQ(book.GetOrderCount(ASK), 7);
    EXPECT_EQ(book.GetOrderCount(BID), 7);

    book.RemoveOrder(order1);
    book.RemoveOrder(order2);

    EXPECT_EQ(book.GetPricePointOrderCount(21, ASK), 3);
    EXPECT_EQ(book.GetPricePointOrderCount(11, BID), 0);
    EXPECT_EQ(book.GetOrderCount(ASK), 6);
    EXPECT_EQ(book.GetOrderCount(BID), 6);
}

TEST(OrderBookTests, OrderBookMultipleOrderRemovalWorks) {
    std::deque<Order> orders_1 = {
            Order(10, 20, 2, BID, NORMAL),
            Order(10, 20, 2, BID, NORMAL),
            Order(10, 20, 2, BID, NORMAL),
            Order(10, 20, 2, BID, NORMAL),
            Order(10, 20, 2, BID, NORMAL),
    };

    std::deque<Order> orders_2 = {
            Order(10, 20, 2, ASK, NORMAL),
            Order(10, 20, 2, ASK, NORMAL),
            Order(10, 20, 2, ASK, NORMAL),
            Order(10, 20, 2, ASK, NORMAL),
    };

    OrderBook book;
    book.AddOrder(Order(20,10,30, ASK, NORMAL));
    book.AddOrder(Order(20,10,30, BID, NORMAL));
    book.RemoveOrders(orders_1);
    EXPECT_EQ(book.GetOrderCount(BID), 1);
    EXPECT_EQ(book.GetOrderCount(ASK), 1);
}

TEST(OrderBookTests, OrderBookBidAskSpreadWorks) {
    std::deque<Order> orders_1 = {
            Order(10, 20, 2, BID, NORMAL),
            Order(12, 20, 2, BID, NORMAL),
            Order(13, 20, 2, BID, NORMAL),
            Order(14, 20, 2, BID, NORMAL),
            Order(15, 20, 2, BID, NORMAL),
    };

    std::deque<Order> orders_2 = {
            Order(20, 20, 2, ASK, NORMAL),
            Order(21, 20, 2, ASK, NORMAL),
            Order(22, 20, 2, ASK, NORMAL),
            Order(23, 20, 2, ASK, NORMAL),
    };

    OrderBook book;
    book.AddOrders(orders_1);
    book.AddOrders(orders_2);
    auto bidask = book.GetBidAsk();
    EXPECT_EQ(5, bidask.spread);
    EXPECT_EQ(15, bidask.best_bid);
    EXPECT_EQ(20, bidask.best_ask);
}
TEST(OrderBookTests, OrderBookBidAskMissingBidWorks) {
    OrderBook book;
    book.AddOrder(Order(15, 20, 2, BID, NORMAL));
    auto bidask = book.GetBidAsk();
    EXPECT_EQ(-1, bidask.spread);
    EXPECT_EQ(15, bidask.best_bid);
    EXPECT_EQ(0, bidask.best_ask);
    book.AddOrder(Order(20, 20, 2, ASK, NORMAL));
}