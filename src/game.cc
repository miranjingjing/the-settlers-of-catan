#include "game.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "dice.h"

Game::Game(std::mt19937 gen, std::default_random_engine rng_input) : turn{0}, game_gen{gen}, game_rng{rng_input} {
    fair = new RandomDice();
    load = new LoadedDice();
    players.resize(4);
    players[0] = new Player(Colour::Blue, load);
    players[1] = new Player(Colour::Red, load);
    players[2] = new Player(Colour::Orange, load);
    players[3] = new Player(Colour::Yellow, load);
    g = new Grid(rng_input);
}

Game::Game(std::mt19937 gen, std::default_random_engine rng_input, std::ifstream &f, bool new_game) :  game_gen{gen}, game_rng{rng_input} {
    fair = new RandomDice();
    load = new LoadedDice();
    players.resize(4);
    // all players start with loaded dice
    players[0] = new Player(Colour::Blue, load);
    players[1] = new Player(Colour::Red, load);
    players[2] = new Player(Colour::Orange, load);
    players[3] = new Player(Colour::Yellow, load);
    if (new_game) {
        turn = 0;
        std::string line;
        std::getline(f, line);
        std::istringstream s{line};
        g = new Grid(s);
    } else {
        std::string colour;
        f >> colour;
        if (colour == "BLUE") {
            turn = 0;
        } else if (colour == "RED") {
            turn = 1;
        } else if (colour == "ORANGE") {
            turn = 2;
        } else if (colour == "YELLOW") {
            turn = 3;
        }
        std::string line;
        // to move to nextline after the turn
        std::getline(f, line);
        for (int i = 0; i < 4; i++) {
            std::getline(f, line);
            std::istringstream s{line};
            players[i]->update_player_by_file(s);
        }
        std::getline(f, line);
        std::istringstream s{line};
        g = new Grid(s);
        // now that grid has been instantiated, manually update grid based on updated player information
        // we can also update the building points of the players through here
        for (int i = 0; i < 4; i++) {
            for (auto edge_id : players[i]->get_roads()) {
                g->build_road(players[i], edge_id);
            }
            for (auto kv : players[i]->get_buildings()) {
                size_t node_id = kv.first;
                Building_Type type = kv.second;
                int num_upgrades = static_cast<int>(type);
                g->build_building(players[i], node_id);
                num_upgrades--;
                while (num_upgrades >= 1) {
                    g->upgrade_building(players[i], node_id);
                    num_upgrades--;
                }
            }
        }
        size_t goose_tile;
        f >> goose_tile;
        g->set_goose(goose_tile);
    }
}

void Game::save_game(std::ofstream &f) {
    // can't use operator<< because that has been set to output the colour codes
    if (turn == 0) {
        f << "BLUE" << std::endl;
    } else if (turn == 1) {
        f << "RED" << std::endl;
    } else if (turn == 2) {
        f << "RED" << std::endl;
    } else if (turn == 3) {
        f << "RED" << std::endl;
    }
    for (auto p : players) {
        p->output_status_to_file(f);
    }
    g->save_board(f);
    f << g->get_goose_tile();
}

void Game::play(bool play_beginning) {
    Player *p;  // keep track of whose turn it is

    if (play_beginning) {
        int curr_player;
        // Beginning of Game
        g->print_grid();
        for (int i = 0; i < 8; i++) {  // get initial basements from players
            curr_player = (i > 3) ? 7 - i : i;
            p = players[curr_player];

            std::cout << "Builder " << p->get_Colour() << ", where do you want to build a basement?\n> ";

            size_t node;  // building vertex
            std::cin >> node;
            while (!g->valid_building(p->get_Colour(), node, true)) {  // invalid building placement
                std::cout << "Cannot build there. Please try again.\n> ";
                std::cin >> node;
            }
            g->build_building(p, node);
        }
    }

    // Actual Game Loop
    bool somebody_has_won = false;
    // end when a player has at least 10 points
    while (true) {
        p = players[turn % 4];

        g->print_grid();
        std::cout << "Builder " << p->get_Colour() << "'s turn." << std::endl;
        p->print_status();

        // Beginning of turn phase
        size_t roll;
        while (true) {
            std::cout << "Current dice are " << ((p->getDice() == fair) ? "fair" : "load") << ". "
                      << "Enter \"load\" to change current dice to loaded dice, \"fair\" to change current dice to fair dice, or \"roll\" to roll the current dice.\n> ";
            std::string cmd;
            std::cin >> cmd;
            std::cout << std::endl;
            if (cmd == "fair") {  // fair dice
                p->setDice(fair);
                // current_dice = fair;
            } else if (cmd == "load") {  // loaded dice
                p->setDice(load);
                // current_dice = load;
            } else if (cmd == "roll")
                break;
        }
        roll = p->roll_dice(game_gen);  // roll dice
        std::cout << "You have rolled a " << roll << ".\n"
                  << std::endl;

        if (roll == 7) {  // roll 7 --> activate geese
            for (auto p : players)
                p->lose_resource_to_geese(game_rng);  // lose resources if more than 10

            int new_geese_loc = g->move_goose();  // move goose
            std::cout << "The GEESE have been moved to " << new_geese_loc << "." << std::endl;

            int victim = g->who_to_steal_from(new_geese_loc, p);
            if (victim >= 0) {
                p->steal(players[victim], game_gen);
            }

        } else {
            g->update_by_roll(roll);
        }

        // During the turn phase
        std::string turn_cmd;
        bool used_drc;

        used_drc = false;

        while (turn_cmd != "next") {  // end turn given command "next"
            std::cout << "Game phase. Please enter a command (enter \"help\" to show valid commands).\n> ";
            std::cin >> turn_cmd;

            if (turn_cmd == "board") {  // prints current board
                g->print_grid();
            } else if (turn_cmd == "status") {  // prints status of all builders in order
                for (auto p : players)
                    p->print_status();
            } else if (turn_cmd == "residences") {  // prints residences the current builder has completed
                p->print_buildings();
            } else if (turn_cmd == "build-road") {  // attempts to build road
                int edge;
                std::cin >> edge;
                if (!p->can_buy_road()) {
                    std::cout << "You do not have enough resources." << std::endl;
                } else if (!g->valid_road(p->get_Colour(), edge)) {
                    std::cout << "You cannot build here." << std::endl;
                } else {
                    g->build_road(p, edge);
                    p->purchase_road();  // call purchase in game and not grid because g->build_road is called in loading game from saved state
                    std::cout << "Congrats! You have built a road on edge " << edge << "." << std::endl;
                }
            } else if (turn_cmd == "build-res") {  // attempts to build a basement
                int node;
                std::cin >> node;
                if (!p->can_buy_basement()) {
                    std::cout << "You do not have enough resources." << std::endl;
                } else if (!g->valid_building(p->get_Colour(), node, false)) {
                    std::cout << "You cannot build here." << std::endl;
                } else {
                    g->build_building(p, node);
                    // call purchase in game and not grid because g->build_road is called in loading game from saved state
                    p->purchase_basement();
                    std::cout << "Congrats! You have built a basement on vertex " << node << ". "
                              << "You have received 1 building point." << std::endl;
                }
                if (p->win()) {
                    somebody_has_won = true;
                    break;
                }                                // achieved 10 or more points
            } else if (turn_cmd == "improve") {  // attempts to improve residence
                int node;
                std::cin >> node;
                if (node < 0 || node > 53) {
                    std::cout << "You cannot build here." << std::endl;
                } else {
                    Building_Type cur_type = g->get_building_type_at_node(node);
                    if (cur_type == Building_Type::Basement) {
                        if (!p->can_buy_house()) {
                            std::cout << "You do not have enough resources." << std::endl;
                        } else if (!g->valid_upgrade(p->get_Colour(), node)) {
                            std::cout << "You cannot build here." << std::endl;
                        } else {
                            g->upgrade_building(p, node);
                            // call purchase in game and not grid because g->build_road is called in loading game from saved state
                            p->purchase_house();
                            std::cout << "Congrats! You have improved your Basement to a House on vertex " << node << ". "
                                      << "You have received 1 building point." << std::endl;
                        }
                    } else if (cur_type == Building_Type::House) {
                        if (!p->can_buy_tower()) {
                            std::cout << "You do not have enough resources." << std::endl;
                        } else if (!g->valid_upgrade(p->get_Colour(), node)) {
                            std::cout << "You cannot build here." << std::endl;
                        } else {
                            g->upgrade_building(p, node);
                            // call purchase in game and not grid because g->build_road is called in loading game from saved state
                            p->purchase_tower();
                            std::cout << "Congrats! You have improved your House to a Tower on vertex " << node << ". "
                                      << "You have received 1 building point." << std::endl;
                        }
                    } else {
                        // cur type is a NoBuilding or tower, so invalid upgrade
                        std::cout << "You cannot build here." << std::endl;
                    }
                    if (p->win()) {
                        somebody_has_won = true;
                        break;
                    }  // achieved 10 or more points
                }
            } else if (turn_cmd == "trade") {  // attempts to trade
                std::string other, give, gain;
                int num_give, num_gain;                                     // how many resources
                std::cin >> other >> num_give >> give >> num_gain >> gain;  // other player in trade, resources being traded

                transform(other.begin(), other.end(), other.begin(), toupper);  // converts to uppercase
                transform(give.begin(), give.end(), give.begin(), toupper);
                transform(gain.begin(), gain.end(), gain.begin(), toupper);

                // converting resource input to resource enum
                Resource resource_to_give, resource_to_gain;  // resources being traded
                if (give == "BRICK")
                    resource_to_give = Resource::Brick;
                else if (give == "ENERGY")
                    resource_to_give = Resource::Energy;
                else if (give == "GLASS")
                    resource_to_give = Resource::Glass;
                else if (give == "HEAT")
                    resource_to_give = Resource::Heat;
                else if (give == "WIFI")
                    resource_to_give = Resource::Wifi;
                if (gain == "BRICK")
                    resource_to_gain = Resource::Brick;
                else if (gain == "ENERGY")
                    resource_to_gain = Resource::Energy;
                else if (gain == "GLASS")
                    resource_to_gain = Resource::Glass;
                else if (gain == "HEAT")
                    resource_to_gain = Resource::Heat;
                else if (gain == "WIFI")
                    resource_to_gain = Resource::Wifi;
                // converting colour input to colour enum
                Colour other_colour;
                if (other == "BLUE") {
                    other_colour = Colour::Blue;
                } else if (other == "RED") {
                    other_colour = Colour::Red;
                } else if (other == "ORANGE") {
                    other_colour = Colour::Orange;
                } else if (other == "YELLOW") {
                    other_colour = Colour::Yellow;
                }

                if (p->get_Colour() == other_colour) {
                    std::cout << "Invalid command. Cannot trade with oneself. " << std::endl;
                } else if (!p->valid_trade_offer(num_give, resource_to_give)) {  // does not possess sufficient resources to give
                    std::cout << "You do not have enough resources." << std::endl;
                } else {
                    std::cout << p->get_Colour() << " offers " << other_colour << " " <<num_give<<" " << give << " " <<num_gain<<" " << gain << "." << std::endl;
                    std::cout << "Does " << other_colour << " accept this offer?\n> ";  // trade offer
                    std::string reply;
                    std::cin >> reply;
                    transform(reply.begin(), reply.end(), reply.begin(), toupper);
                    if (reply == "NO") {  // no
                        std::cout << "Trade declined." << std::endl;
                    } else {
                        int trade_other;
                        if (other == "BLUE") {  // trade with blue
                            trade_other = 0;
                        } else if (other == "RED") {  // trade with red
                            trade_other = 1;
                        } else if (other == "ORANGE") {  // trade with orange
                            trade_other = 2;
                        } else if (other == "YELLOW") {  // trade with yellow
                            trade_other = 3;
                        }
                        if (!players[trade_other]->valid_trade_acceptance(num_gain, resource_to_gain)) {
                            std::cout << other_colour << " does not have enough resources to make the trade." << std::endl;
                        } else {
                            p->trade_resources(players[trade_other], num_give, resource_to_give, num_gain, resource_to_gain);
                        }
                    }
                }
            } else if (turn_cmd == "buy-drc") {  // passes control to next player
                if (p->can_buy_drc()) {
                    p->purchase_drc(game_gen);
                } else {
                    std::cout << p->get_Colour() << " does not have enough resources to buy a drc." << std::endl;
                }
            } else if (turn_cmd == "print-drc") {  // passes control to next player
                p->print_drc();
            } else if (turn_cmd == "use-knight") {  // passes control to next player
                if (!p->has_drc(0)) {
                    std::cout << p->get_Colour() << " does not have a knight drc." << std::endl;
                } else if (used_drc) {
                    std::cout << "Only one drc per turn." << std::endl;
                } else {
                    used_drc = true;
                    p->increment_drc(0, -1);

                    int new_geese_loc = g->move_goose();  // move goose
                    std::cout << "The GEESE have been moved to " << new_geese_loc << "." << std::endl;

                    int victim = g->who_to_steal_from(new_geese_loc, p);
                    if (victim >= 0) {
                        p->steal(players[victim], game_gen);
                    }
                }
            } else if (turn_cmd == "use-year-of-plenty") {  // passes control to next player
                if (!p->has_drc(1)) {
                    std::cout << p->get_Colour() << " does not have a year of plenty drc." << std::endl;
                } else if (used_drc) {
                    std::cout << "Only one drc per turn." << std::endl;
                } else {
                    used_drc = true;
                    p->increment_drc(1, -1);

                    std::cout << "Choose to recieve 2 of a resource." << std::endl;
                    std::string rec;
                    std::cin >> rec;  // other player in trade, resources being traded

                    transform(rec.begin(), rec.end(), rec.begin(), toupper);
                    std::cout << p->get_Colour() << " has gained " << 2 << " ";

                    if (rec == "BRICK") {
                        p->increment_resource(0, 2);
                        std::cout << "bricks" << std::endl;
                    } else if (rec == "ENERGY") {
                        p->increment_resource(1, 2);
                        std::cout << "energy" << std::endl;
                    } else if (rec == "GLASS") {
                        p->increment_resource(2, 2);
                        std::cout << "glass" << std::endl;
                    } else if (rec == "HEAT") {
                        p->increment_resource(3, 2);
                        std::cout << "heat" << std::endl;
                    } else {
                        p->increment_resource(4, 2);
                        std::cout << "wifi" << std::endl;
                    }
                }

            } else if (turn_cmd == "use-monopoly") {  // passes control to next player
                if (!p->has_drc(2)) {
                    std::cout << p->get_Colour() << " does not have a monopoly drc." << std::endl;
                } else if (used_drc) {
                    std::cout << "Only one drc per turn." << std::endl;
                } else {
                    used_drc = true;
                    p->increment_drc(2, -1);

                    std::cout << "What resource would you like to monopolize?" << std::endl;
                    std::string rec;
                    std::cin >> rec;  // other player in trade, resources being traded

                    transform(rec.begin(), rec.end(), rec.begin(), toupper);
                    int resource, total = 0;
                    Resource recstr;

                    if (rec == "BRICK") {
                        resource = 0;
                        recstr = Resource::Brick;
                    } else if (rec == "ENERGY") {
                        resource = 1;
                        recstr = Resource::Energy;
                    } else if (rec == "GLASS") {
                        resource = 2;
                        recstr = Resource::Glass;
                    } else if (rec == "HEAT") {
                        resource = 3;
                        recstr = Resource::Heat;
                    } else {
                        resource = 4;
                        recstr = Resource::Wifi;
                    }

                    for (auto other : players) {
                        if (p != other) {
                            total += other->get_resource_count(resource);
                            other->increment_resource(resource, -1 * other->get_resource_count(resource));
                        }
                    }
                    p->increment_resource(resource, total);
                    std::cout << p->get_Colour() << " has gained " << total << " " << recstr;
                }
            } else if (turn_cmd == "use-victory-point") {  // passes control to next player
                if (!p->has_drc(3)) {
                    std::cout << p->get_Colour() << " does not have a victory point drc." << std::endl;
                } else if (used_drc) {
                    std::cout << "Only one drc per turn." << std::endl;
                } else {
                    used_drc = true;
                    p->increment_drc(3, -1);

                    p->increment_points();
                    std::cout << "Congrats! You have received 1 building point." << std::endl;
                }

                if (p->win()) {
                    somebody_has_won = true;
                    break;
                }
            } else if (turn_cmd == "use-road-building") {  // passes control to next player
                if (!p->has_drc(4)) {
                    std::cout << p->get_Colour() << " does not have a road building drc." << std::endl;
                } else if (used_drc) {
                    std::cout << "Only one drc per turn." << std::endl;
                } else {
                    used_drc = true;
                    p->increment_drc(4, -1);

                    size_t edge;
                    std::cout << "Where do you want to place a road?" << std::endl;
                    std::cin >> edge;

                    while (!g->valid_road(p->get_Colour(), edge)) {
                        std::cout << "You cannot build here. Choose again." << std::endl;
                        std::cin >> edge;
                    }
                    g->build_road(p, edge);
                    std::cout << "Congrats! You have built a road on edge " << edge << "." << std::endl;
                    std::cout << "Where do you want to place a road?" << std::endl;
                    std::cin >> edge;

                    while (!g->valid_road(p->get_Colour(), edge)) {
                        std::cout << "You cannot build here. Choose again." << std::endl;
                        std::cin >> edge;
                    }
                    g->build_road(p, edge);
                    std::cout << "Congrats! You have built a road on edge " << edge << "." << std::endl;
                }
            } else if (turn_cmd == "next") {  // passes control to next player
                continue;
            } else if (turn_cmd == "save") {  // saves current game state
                std::string file_name;
                std::cin >> file_name;
                std::ofstream file_stream{file_name};
                save_game(file_stream);
            } else if (turn_cmd == "help") {  // prints out list of commands
                std::cout << "Valid commands: " << std::endl;
                std::cout << "board" << std::endl;
                std::cout << "status" << std::endl;
                std::cout << "residences" << std::endl;
                std::cout << "build-road <edge#>" << std::endl;
                std::cout << "build-res <housing#>" << std::endl;
                std::cout << "improve <housing#>" << std::endl;
                std::cout << "trade <colour> <give> <take>" << std::endl;
                std::cout << "buy-drc" << std::endl;
                std::cout << "print-drc" << std::endl;
                std::cout << "use-knight" << std::endl;
                std::cout << "use-year-of-plenty" << std::endl;
                std::cout << "use-monopoly" << std::endl;
                std::cout << "use-victory-point" << std::endl;
                std::cout << "use-road-building" << std::endl;
                std::cout << "next" << std::endl;
                std::cout << "save <file>" << std::endl;
                std::cout << "help" << std::endl;
            } else {  // invalid command
                std::cout << "Invalid command." << std::endl;
            }
            std::cout << std::endl;
        }

        turn = (turn + 1) % 4;
        if (somebody_has_won) {
            break;
        }
    }
    std::cout << "Congrats! Builder " << p->get_Colour() << " has earned 10 points and won the game!" << std::endl;
}

Game::~Game() {
    for (auto p : players)
        delete p;
    delete g;
    delete fair;
    delete load;
}
