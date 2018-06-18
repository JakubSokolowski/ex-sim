//
// Created by jakub on 14.06.18.
//

#ifndef HESOYAM_PLAYER_H
#define HESOYAM_PLAYER_H

#include <thread>
#include <deque>
#include <random>
#include <string>
#include "Coordinator.h"
#include "Order.h"
#include "SafeQueue.h"
#include "EventGenerator.h"
#include "ExchangeChannel.h"
#include "Printer.h"
#include "Visualisation.h"

using uint = unsigned int;
enum PlayerState {
    MAKING_ORDER,
    MAKING_ASK,
    WAITING,
    DECIDING
};

enum PlayerDecision {
    BUY,
    SELL,
    HOLD
};
class Player {
public:

    Player(int id, uint money, uint stock_amount, EventGenerator &evg, ExchangeChannel &channel, Visualisation &vis);
    Player(Player &&other) noexcept;
    ~Player(){
        if(player_thread_m.joinable())
            player_thread_m.join();
    }


    int id_m = -1;
    uint money_m;
    uint stock_amount_m;
    bool first_order = true;

    ExchangeChannel &exchange_channel;
    EventGenerator &generator;
    Visualisation &visualisation;
    std::thread player_thread_m;

    std::deque<Order> active_orders_m;

    PlayerType type_m = NORMAL;
    PlayerState player_state = WAITING;
    PlayerDecision MakeDecision(Event evnt);
    PlayerDecision ProbableDecision(int buy_p, int sell_p, int hold_p);
    void Play();
    void MakeOrder(Order order);
    void MakeDecision();
    std::string GetId();
    bool CanBuy();
    bool CanSell();
    void PrintStatus();
    std::string GetStatus();
    void SetRandomStatus();
    Order GenerateOrder(PlayerDecision decision);
    void UpdateState(PlayerState state);
    Order BuyOrder();
    Order SellOrder();

    Order EmptyOrder();
    std::string ToString(Order);
    std::mt19937 rng_m{ std::random_device{}()};
    void WaitRandom(int low, int up);
    int RandomInRange(int low, int up);

    bool Happened(float prob);
};


#endif //HESOYAM_PLAYER_H
