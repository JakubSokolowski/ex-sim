#include <thread>
#include "Visualisation.h"
#include "EventGenerator.h"

Visualisation::Visualisation(CommChannel &channel)
: channel_m(channel) {

}

void Visualisation::UpdateTableInfo(vector<string> id_vec, vector<string> state_vec) {
    player_ids_m = id_vec;
    player_states_m = std::move(state_vec);
    player_items_m = new ITEM*[id_vec.size()];
    auto num = player_ids_m.size();
    for(uint i = 0; i < num; ++i)
        player_items_m[i] = new_item(player_ids_m[i].c_str(), player_states_m[i].c_str());
    Setup();
    CreatePhilMenu();
}

void Visualisation::UpdatePhilInfo(int id, string name, string desc) {
    std::lock_guard<std::mutex> lock_guard(window_lock_m);
    player_ids_m[id] = std::move(name);
    player_states_m[id] = std::move(desc);
    player_items_m[id] = new_item(player_ids_m[id].c_str(), player_states_m[id].c_str());

    // Prevent cursor from moving to the top of the menu
    auto current = player_menu->curitem;
    unpost_menu(player_menu);
    set_menu_items(player_menu,player_items_m);
    set_current_item(player_menu, current);
    post_menu(player_menu);
    wrefresh(player_menu_win);
}

void Visualisation::Start() {
    int option;
    bool finished = false;

    while((option = wgetch(player_menu_win)) && !finished )
    {
        std::lock_guard<std::mutex> lock_guard(window_lock_m);
        switch(option)
        {
            case KEY_DOWN:
                menu_driver(player_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(player_menu, REQ_UP_ITEM);
                break;
            case KEY_NPAGE:
                menu_driver(player_menu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                menu_driver(player_menu, REQ_SCR_UPAGE);
                break;
            case 113:
                SendKillSignal();
                finished = true;
                break;
            default:
                break;
        }
        wrefresh(player_menu_win);
    }
}

Visualisation::~Visualisation() {
    wrefresh(player_menu_win);
    Teardown();
}

void Visualisation::Setup() {
    /* Initialize curses */
    initscr();
    getmaxyx(stdscr, lines, cols);
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, true);
    use_default_colors();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
}

void Visualisation::Teardown() {
    // Unpost and free all the memory taken up
    unpost_menu(player_menu);
    free_menu(player_menu);
    for(uint i = 0; i < player_ids_m.size(); ++i)
        free_item(player_items_m[i]);
    endwin();
}

void Visualisation::CreatePhilMenu() {
    /* Crate menu */
    player_menu = new_menu(player_items_m);
    /* Create the window to be associated with the menu */
    // lines, cols, begin y, begin x
    player_menu_win = newwin(lines - 5, cols - 10, 1, 4);
    keypad(player_menu_win, true);

    /* Set main window and sub window */
    set_menu_win(player_menu, player_menu_win);
    set_menu_sub(player_menu, derwin(player_menu_win, lines - 10, cols - 15, 3, 1));
    // rows,cols
    set_menu_format(player_menu, lines - 10, 1);

    /* Set menu mark to the string " * " */
    set_menu_mark(player_menu, "");

    /* Print a border around the main window and print a title */
    box(player_menu_win, 0, 0);
    mvwprintw(player_menu_win, 1, 4, "%s", "Players");
    refresh();
    mvwaddch(player_menu_win, 2, 0, ACS_LTEE);
    mvwhline(player_menu_win, 2, 1, ACS_HLINE, cols - 10);
    mvwaddch(player_menu_win, 2, cols - 11, ACS_RTEE);

    /* Post the menu */
    post_menu(player_menu);
    wrefresh(player_menu_win);

    refresh();
}

void Visualisation::UpdateExchangeState(std::string info) {
    std::lock_guard<std::mutex> lock_guard(window_lock_m);
    mvprintw(LINES - 3, 4, "%s", info.c_str());
    wrefresh(player_menu_win);
    refresh();
}

void Visualisation::UpdateEvent(Event event) {
    std::lock_guard<std::mutex> lock_guard(window_lock_m);
    std::string str;
    switch(event.type) {

        case GOOD:
            str = "Good    Event";
            break;
        case BAD:
            str = "Bad     Event";
            break;
        case NEUTRAL:
            str = "Neutral Event";
            break;
    }
    mvprintw(LINES - 2, 4, "%s", str.c_str());
    wrefresh(player_menu_win);
    refresh();
}

