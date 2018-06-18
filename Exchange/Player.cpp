#include "Player.h"
#include <thread>
Player::Player(int id, uint money, uint stock_amount, EventGenerator &evg, ExchangeChannel &channel, Visualisation &vis)
        : id_m(id)
        , money_m(money)
        , stock_amount_m(stock_amount)
        , exchange_channel(channel)
        , generator(evg)
        , visualisation(vis)
        , player_thread_m(&Player::Play, this)
{}

Player::Player(Player &&other) noexcept
    : id_m(other.id_m)
    , money_m(other.money_m)
    , stock_amount_m(other.stock_amount_m)
    , exchange_channel(other.exchange_channel)
    , generator(other.generator)
    , visualisation(other.visualisation)
    , player_thread_m(std::move(other.player_thread_m))
{}

void Player::Play() {
//    while (exchange_channel.channel_m.exchange_closed)
//        exchange_channel.channel_m.coordinator.Wait();
    exchange_channel.channel_m.coordinator.Wait();
    while(!exchange_channel.channel_m.exchange_closed) {
        MakeDecision();
        for(int i = 0; i < 5 ; i++) {
            WaitRandom(200, 500);
        }
    }
}

void Player::MakeOrder(Order order) {
    if(order.player_id_m == -1) {
        visualisation.UpdatePhilInfo(id_m, GetId(), "Holds");
        return;
    }

    if(type_m == INSIDER)
        exchange_channel.order_queue.PushFront(order);
    else
        exchange_channel.order_queue.PushBack(order);
    visualisation.UpdatePhilInfo(id_m, GetId(), ToString(order));

}

void Player::MakeDecision() {
    visualisation.UpdatePhilInfo(id_m, GetId(), "Decides...");
    auto decision = MakeDecision(generator.CurrentEvent());
    auto order = GenerateOrder(decision);
    MakeOrder(order);
}


void Player::PrintStatus() {
   ThreadSafePrinter::Print(GetStatus());

}

void Player::UpdateState(PlayerState state) {
    player_state = state;
}

void Player::WaitRandom(int low, int up) {
    static thread_local std::uniform_int_distribution<> dist(low, up);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng_m)));
}

int Player::RandomInRange(int low, int up) {
    std::uniform_int_distribution<> dist(low, up);
    return dist(rng_m);
}

std::string Player::GetStatus() {
    std::string status;
    switch(player_state) {
        case MAKING_ORDER:
            return  "Making order";
        case MAKING_ASK:
            return  "Making ask";
        case WAITING:
            return "Waiting";
        case DECIDING:
            return  "Deciding";
    }
    return status;
}

void Player::SetRandomStatus() {
    player_state = static_cast<PlayerState >(RandomInRange(0,3));
}

std::string Player::GetId() {
    int net_worth = money_m + stock_amount_m * exchange_channel.engine.current_stock_price_m;
    std::string result = type_m == INSIDER ? "PI" : "P";
    return result + std::to_string(id_m) + " $" + std::to_string(money_m) + " Stock :" + std::to_string(stock_amount_m)
           + " Networth " + std::to_string(net_worth) + "$";
}

bool Player::CanBuy() {
    return money_m > 0 ;
           //&& exchange_channel.engine.book.GetBidAsk().best_ask > 0;
}

bool Player::CanSell() {
    return stock_amount_m > 0;
           //&&  exchange_channel.engine.book.GetBidAsk().best_bid > 0;
}


PlayerDecision Player::MakeDecision(Event evnt) {
    switch(evnt.type) {
        case GOOD:
            return ProbableDecision(2,1,1);
        case BAD:
            return ProbableDecision(1,10,1);
        case NEUTRAL:
            break;
    }
    return ProbableDecision(1,1,1);
}

PlayerDecision Player::ProbableDecision(int buy_p, int sell_p, int hold_p) {
    std::vector<PlayerDecision> decisions;
    for(int i = 0; i < buy_p; i++)
        decisions.push_back(BUY);
    for(int i = 0; i < sell_p; i++)
        decisions.push_back(SELL);
    for(int i = 0; i < hold_p; i++)
        decisions.push_back(HOLD);
    return decisions[RandomInRange(0, (int)decisions.size() -1)];
}

Order Player::GenerateOrder(PlayerDecision decision) {
    switch(decision)  {
        case BUY:
            return BuyOrder();
        case SELL:
            return SellOrder();
        case HOLD:
            break;
    }
    return EmptyOrder();
}

Order Player::BuyOrder() {
    if(first_order) {
        first_order = false;
        return Order(RandomInRange(10,20), RandomInRange(10, 15),id_m, BID, type_m);
    }
    if(CanBuy()) {
        bool sell_at_market_price = Happened(0.2);
        int market_price = exchange_channel.engine.book.GetBidAsk().best_ask;
        if(market_price == 0)
            market_price = 10;
        int max_stock_amount = money_m / market_price;
        int upper = max_stock_amount / 4 > 0 ? max_stock_amount / 4: 1;
        if(sell_at_market_price) {
            int amount = RandomInRange(1, upper);
            money_m -= amount * market_price;
            return Order(market_price, amount, id_m, BID, type_m);
        }
        else {
            // decrease  buying price a little
            if(market_price > 5)
                market_price -= RandomInRange(1,5);
            max_stock_amount = money_m / market_price;
            upper = max_stock_amount / 4 > 0 ? max_stock_amount / 4: 1;
            int amount = RandomInRange(1, upper);
            money_m -= amount * market_price;
            return Order(market_price, amount, id_m, BID, type_m);
        }
    }
    return EmptyOrder();
}

Order Player::SellOrder() {
    if(first_order) {
        first_order = false;
        int amount = RandomInRange(10, 15);
        stock_amount_m -= amount;
        return Order(RandomInRange(30,40), amount, id_m, ASK, type_m);
    }
    if(CanSell()) {
        bool sell_at_market_price = Happened(0.2);
        int market_price = exchange_channel.engine.book.GetBidAsk().best_bid;
        if(market_price == 0)
            market_price = 1;
        int upper = stock_amount_m / 4 > 0 ? stock_amount_m / 4: 1;
        if(sell_at_market_price) {
            int amount = RandomInRange(1,upper);
            stock_amount_m -= amount;
            return Order(market_price,  amount, id_m, ASK, type_m);
        }
        else {
            // increase selling price by little
            market_price += RandomInRange(1,5);
            int amount = RandomInRange(1,upper);
            stock_amount_m -= amount;
            return Order(market_price, amount, id_m, ASK, type_m);
        }
    }
    return EmptyOrder();
}

Order Player::EmptyOrder() {
    return Order(0, 0, -1, BID, NORMAL);
}

std::string Player::ToString(Order order) {
    std::string result = order.order_type_m == BID ? "Bids " : "Asks ";
    result += std::to_string(order.amount_m)
            + " stock for " + std::to_string(order.price_m)
            + "$ ID: " + std::to_string(order.id_m);
    return result;
}

bool Player::Happened(float prob) {
    std::bernoulli_distribution d(prob);
    return d(rng_m);
}






