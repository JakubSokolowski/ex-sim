#include <sstream>
#include <functional>
#include "StockExchange.h"
#include "Printer.h"

StockExchange::StockExchange(uint player_count)
        : channel(comms, engine)
        , generator(comms)
        , visualisation(comms)
{
    CreatePlayers(player_count);
    visualisation.UpdateTableInfo(GetPlayerIds(), GetPlayerStatus());
}

void StockExchange::MakeOrder(Order order) {
    DistributePayout(engine.MatchOrder(order));
}

void StockExchange::CreatePlayers(uint player_count) {
    for(uint i = 0; i < player_count; i++) {
        players.emplace_back(i, 10000, 500, generator, channel, visualisation);
        if(players.back().Happened(0.2))
            players.back().type_m = INSIDER;
    }
}

void StockExchange::DistributePayout(std::deque<Payout> payout) {
    for(auto pay : payout) {
        players[pay.to_id].stock_amount_m += pay.stock_amount;
        if (pay.stock_amount != 0)
            visualisation.UpdatePhilInfo(pay.to_id, players[pay.to_id].GetId(), "Received " + std::to_string(pay.stock_amount) + " stock ID: " + std::to_string(pay.order_id));

        players[pay.to_id].money_m += pay.funds;
        if (pay.funds != 0)
            visualisation.UpdatePhilInfo(pay.to_id, players[pay.to_id].GetId(), "Received " + std::to_string(pay.funds) + "$ ID: "  + std::to_string(pay.order_id));
    }
}

void StockExchange::Run() {
    // Start the player threads
    // comms.coordinator.NotifyAll();
    channel.channel_m.exchange_closed = false;
    channel.channel_m.coordinator.NotifyAll();
    std::thread vis_thread([&]{visualisation.Start();});
    while(!channel.channel_m.exchange_closed) {
        visualisation.UpdateExchangeState(GetExchangeInfo());
        visualisation.UpdateEvent(generator.CurrentEvent());
        if(channel.order_queue.IsEmpty())
            continue;
        auto order = channel.order_queue.PopFront();
        DistributePayout(engine.MatchOrder(order));
    }
    vis_thread.join();
}



void StockExchange::DisplayStatus() {
    std::string status;
    for(auto &player : players) {
        status += player.GetStatus();
    }
    ThreadSafePrinter::Print(status);
}

void StockExchange::Stop() {
    comms.exchange_closed = true;
}

vector<string> StockExchange::GetPlayerIds() {
    auto out = std::vector<std::string>();
    for(auto &player : players)
        out.emplace_back(player.GetId());
    out.emplace_back("\0");
    return out;
}

vector<string> StockExchange::GetPlayerStatus() {
    auto out = std::vector<std::string>();
    for(auto &player : players)
        out.emplace_back(player.GetStatus());
    // Items in ncurses menu need to be null terminated
    out.emplace_back("\0");
    return out;
}

std::string StockExchange::GetExchangeInfo() {
    std::stringstream stream;
    std::string result;
    auto bidask = engine.book.GetBidAsk();
    stream  << "Price: " << engine.current_stock_price_m
            << "$ Bid: " << bidask.best_bid << "$ Ask: " << bidask.best_ask
            << "$ Spread: " << bidask.spread << " Volume: " << engine.volume
            << "$ Bid Orders: " <<engine.book.GetOrderCount(BID)
            << " Ask Orders: " << engine.book.GetOrderCount(ASK);
//    << " Q size: " << channel.order_queue.Size();
    return stream.str();
}
