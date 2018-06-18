//
// Created by jakub on 17.06.18.
//

#ifndef HESOYAM_VISUALISATION_H
#define HESOYAM_VISUALISATION_H

#include "menu.h"
#include <ncurses.h>
#include <vector>
#include <string>
#include <memory>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <mutex>
#include <atomic>
#include "Coordinator.h"
#include "EventGenerator.h"

using std::string;
using std::vector;

class Visualisation {

public:
    explicit Visualisation(CommChannel &channel);
    ~Visualisation();

    void Start();
    void UpdateTableInfo(vector<string> id_vec, vector<string> state_vec);
    void UpdatePhilInfo(int id, string name, string desc);

    void UpdateExchangeState(std::string info);

    void UpdateEvent(Event event);

private:

    std::mutex window_lock_m;
    std::vector<std::string> player_ids_m;
    std::vector<std::string> player_states_m;

    CommChannel &channel_m;
    ITEM **player_items_m = nullptr;
    MENU * player_menu = nullptr;
    WINDOW* player_menu_win = nullptr;

//    ITEM **fork_items = nullptr;
//    MENU *fork_menu = nullptr;
//    WINDOW* fork_menu_ein = nullptr;

    int lines, cols;

    void Setup();
    void Teardown();
    void CreatePhilMenu();

    void SendKillSignal() { channel_m.exchange_closed = true;}
};


#endif //HESOYAM_VISUALISATION_H
