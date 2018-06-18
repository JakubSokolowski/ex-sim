//
// Created by jakub on 14.06.18.
//

#ifndef HESOYAM_EXCHANGE_H
#define HESOYAM_EXCHANGE_H

#include "MatchingEngine.h"
#include "EventGenerator.h"
#include "Player.h"
#include <vector>
#include <string>
using std::vector;
using std::string;
class StockExchange {
public:
    explicit StockExchange(uint player_count);
    void Run();
    void CreatePlayers(uint player_count);
private:

    void DistributePayout(std::deque<Payout> payout);
    void MakeOrder(Order order);

    CommChannel comms;
    MatchingEngine engine;
    ExchangeChannel channel;
    EventGenerator generator;
    Visualisation visualisation;
    std::deque<Player> players;
    void Stop();

    void WaitRandom(int low, int up) {
        std::mt19937 rng_m{ std::random_device{}()};
        static thread_local std::uniform_int_distribution<> dist(low, up);
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng_m)));
    }
    int RandomInRange(int low, int up) {
        std::mt19937 rng_m{ std::random_device{}()};
        std::uniform_int_distribution<> dist(low, up);
        return dist(rng_m);
    }
    std::string GetExchangeInfo();
    vector<string> GetPlayerIds();
    vector<string> GetPlayerStatus();
    void DisplayStatus();


};


#endif //HESOYAM_EXCHANGE_H
