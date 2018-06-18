//
// Created by jakub on 17.06.18.
//

#include <gtest/gtest.h>
#include <MatchingEngine.h>

TEST(MatchingEngineTests, MatchingEngineOrderCanBeFilledWorks) {
    MatchingEngine engine;
    EXPECT_FALSE(engine.CanBeFilled(Order(21,10,1,ASK,NORMAL)));
    EXPECT_FALSE(engine.CanBeFilled(Order(21,10,1,BID,NORMAL)));
    engine.book.AddOrder(Order(21,10,1,ASK,NORMAL));
    engine.book.AddOrder(Order(21,10,1,BID,NORMAL));
    EXPECT_TRUE(engine.CanBeFilled(Order(21,10,1,ASK,NORMAL)));
    EXPECT_TRUE(engine.CanBeFilled(Order(21,10,1,BID,NORMAL)));

}


TEST(MatchingEngineTests, MatchingEngineBidFullFillWorks) {
    MatchingEngine engine;
    auto payout = engine.MatchOrder(Order(21, 10, 1, ASK, NORMAL));
    EXPECT_EQ(payout.size(), 0);
    engine.MatchOrder(Order(21, 10, 2, ASK, NORMAL));
    engine.MatchOrder(Order(22, 10, 3, ASK, NORMAL));
    payout = engine.MatchOrder(Order(21, 15, 4, BID, NORMAL));
    EXPECT_EQ(payout.size(), 3);
    EXPECT_EQ(payout[0].funds, 210);
    EXPECT_EQ(payout[0].to_id, 1);
    EXPECT_EQ(payout[0].stock_amount, 0);
    EXPECT_EQ(payout[1].funds, 105);
    EXPECT_EQ(payout[1].to_id, 2);
    EXPECT_EQ(payout[1].stock_amount, 0);
    EXPECT_EQ(payout[2].funds, 0);
    EXPECT_EQ(payout[2].to_id, 4);
    EXPECT_EQ(payout[2].stock_amount, 15);
    EXPECT_EQ(engine.book.GetOrderCount(BID), 0);
    EXPECT_EQ(engine.book.GetOrderCount(ASK), 2);
}

TEST(MatchingEngineTests, MatchingEngineBidPartiallFillWorks) {
    MatchingEngine engine;
    auto payout = engine.MatchOrder(Order(21, 10, 1, ASK, NORMAL));
    EXPECT_EQ(payout.size(), 0);
    engine.MatchOrder(Order(21, 10, 2, ASK, NORMAL));
    engine.MatchOrder(Order(22, 10, 3, ASK, NORMAL));
    payout = engine.MatchOrder(Order(21, 22, 4, BID, NORMAL));
    EXPECT_EQ(payout.size(), 3);
    EXPECT_EQ(payout[0].funds, 210);
    EXPECT_EQ(payout[0].to_id, 1);
    EXPECT_EQ(payout[0].stock_amount, 0);
    EXPECT_EQ(payout[1].funds, 210);
    EXPECT_EQ(payout[1].to_id, 2);
    EXPECT_EQ(payout[1].stock_amount, 0);
    EXPECT_EQ(payout[2].funds, 0);
    EXPECT_EQ(payout[2].to_id, 4);
    EXPECT_EQ(payout[2].stock_amount, 20);
    EXPECT_EQ(engine.book.GetOrderCount(BID), 1);
    EXPECT_EQ(engine.book.GetOrderCount(ASK), 1);
}

TEST(MatchingEngineTests, MatchingEngineBidAskFullFillWorks) {
    MatchingEngine engine;
    auto payout = engine.MatchOrder(Order(21, 10, 1, ASK, NORMAL));
    EXPECT_EQ(payout.size(), 0);
    engine.MatchOrder(Order(21, 10, 2, ASK, NORMAL));
    engine.MatchOrder(Order(22, 10, 3, ASK, NORMAL));
    payout = engine.MatchOrder(Order(21, 20, 4, BID, NORMAL));
    EXPECT_EQ(payout.size(), 3);
    EXPECT_EQ(payout[0].funds, 210);
    EXPECT_EQ(payout[0].to_id, 1);
    EXPECT_EQ(payout[0].stock_amount, 0);
    EXPECT_EQ(payout[1].funds, 210);
    EXPECT_EQ(payout[1].to_id, 2);
    EXPECT_EQ(payout[1].stock_amount, 0);
    EXPECT_EQ(payout[2].funds, 0);
    EXPECT_EQ(payout[2].to_id, 4);
    EXPECT_EQ(payout[2].stock_amount, 20);
    EXPECT_EQ(engine.book.GetOrderCount(BID), 0);
    EXPECT_EQ(engine.book.GetOrderCount(ASK), 1);
}

TEST(MatchingEngineTests, MatchingEngineAskFullFillWorks) {
    MatchingEngine engine;
    auto payout = engine.MatchOrder(Order(21, 10, 1, BID, NORMAL));
    EXPECT_EQ(payout.size(), 0);
    engine.MatchOrder(Order(21, 10, 2, BID, NORMAL));
    engine.MatchOrder(Order(22, 10, 3, BID, NORMAL));
    payout = engine.MatchOrder(Order(21, 15, 4, ASK, NORMAL));
    EXPECT_EQ(payout.size(), 3);
    EXPECT_EQ(payout[0].funds, 0);
    EXPECT_EQ(payout[0].to_id, 1);
    EXPECT_EQ(payout[0].stock_amount, 10);
    EXPECT_EQ(payout[1].funds, 0);
    EXPECT_EQ(payout[1].to_id, 2);
    EXPECT_EQ(payout[1].stock_amount, 5);
    EXPECT_EQ(payout[2].funds, 315);
    EXPECT_EQ(payout[2].to_id, 4);
    EXPECT_EQ(payout[2].stock_amount, 0);
    EXPECT_EQ(engine.book.GetOrderCount(BID), 2);
    EXPECT_EQ(engine.book.GetOrderCount(ASK), 0);
}

TEST(MatchingEngineTests, MatchingEngineAskPartiallFillWorks) {
    MatchingEngine engine;
    auto payout = engine.MatchOrder(Order(21, 10, 1, BID, NORMAL));
    EXPECT_EQ(payout.size(), 0);
    engine.MatchOrder(Order(21, 10, 2, BID, NORMAL));
    engine.MatchOrder(Order(22, 10, 3, BID, NORMAL));
    payout = engine.MatchOrder(Order(21, 22, 4, ASK, NORMAL));
    EXPECT_EQ(payout.size(), 3);
    EXPECT_EQ(payout[0].funds, 0);
    EXPECT_EQ(payout[0].to_id, 1);
    EXPECT_EQ(payout[0].stock_amount, 10);
    EXPECT_EQ(payout[1].funds, 0);
    EXPECT_EQ(payout[1].to_id, 2);
    EXPECT_EQ(payout[1].stock_amount, 10);
    EXPECT_EQ(payout[2].funds, 420);
    EXPECT_EQ(payout[2].to_id, 4);
    EXPECT_EQ(payout[2].stock_amount, 0);
    EXPECT_EQ(engine.book.GetOrderCount(BID), 1);
    EXPECT_EQ(engine.book.GetOrderCount(ASK), 1);
}

TEST(MatchingEngineTests, MatchingEngineAskBidFullFillWorks) {
    MatchingEngine engine;
    auto payout = engine.MatchOrder(Order(21, 10, 1, BID, NORMAL));
    EXPECT_EQ(payout.size(), 0);
    engine.MatchOrder(Order(21, 10, 2, BID, NORMAL));
    engine.MatchOrder(Order(22, 10, 3, BID, NORMAL));
    payout = engine.MatchOrder(Order(21, 20, 4, ASK, NORMAL));
    EXPECT_EQ(payout.size(), 3);
    EXPECT_EQ(payout[0].funds, 0);
    EXPECT_EQ(payout[0].to_id, 1);
    EXPECT_EQ(payout[0].stock_amount, 10);
    EXPECT_EQ(payout[1].funds, 0);
    EXPECT_EQ(payout[1].to_id, 2);
    EXPECT_EQ(payout[1].stock_amount, 10);
    EXPECT_EQ(payout[2].funds, 420);
    EXPECT_EQ(payout[2].to_id, 4);
    EXPECT_EQ(payout[2].stock_amount, 0);
    EXPECT_EQ(engine.book.GetOrderCount(BID), 1);
    EXPECT_EQ(engine.book.GetOrderCount(ASK), 0);
}

TEST(MatchingEngineTests, MatchingEngineOrderBookClearWorks) {
    MatchingEngine engine;
    auto payout = engine.MatchOrder(Order(21, 10, 1, BID, NORMAL));
    EXPECT_EQ(payout.size(), 0);
    engine.MatchOrder(Order(21, 10, 2, BID, NORMAL));
    payout = engine.MatchOrder(Order(21, 20, 4, ASK, NORMAL));
    EXPECT_EQ(payout.size(), 3);
    EXPECT_EQ(payout[0].funds, 0);
    EXPECT_EQ(payout[0].to_id, 1);
    EXPECT_EQ(payout[0].stock_amount, 10);
    EXPECT_EQ(payout[1].funds, 0);
    EXPECT_EQ(payout[1].to_id, 2);
    EXPECT_EQ(payout[1].stock_amount, 10);
    EXPECT_EQ(payout[2].funds, 420);
    EXPECT_EQ(payout[2].to_id, 4);
    EXPECT_EQ(payout[2].stock_amount, 0);
    EXPECT_EQ(engine.book.GetOrderCount(BID), 0);
    EXPECT_EQ(engine.book.GetOrderCount(ASK), 0);
    EXPECT_EQ(engine.volume, 420);      // Adjust volume summation
    EXPECT_EQ(engine.current_stock_price_m, 21);
}