#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <unordered_map>
#include <utility>
#include <vector>
// for enum resources, building types
#include "components.h"
#include "dice.h"
// for random
#include <chrono>
#include <random>

class Building;

class Player {
    int victory_points = 0;
    std::vector<int> resource_count = {0, 0, 0, 0, 0};
    std::vector<int> drc_count = {0, 0, 0, 0, 0};
    std::vector<std::size_t> roads = std::vector<std::size_t>();
    std::unordered_map<std::size_t, Building_Type> buildings = std::unordered_map<std::size_t, Building_Type>();
    Colour colour;
    // dice
    Dice *dice;

    std::string print_resource(size_t type) const;

   public:
    Player(Colour colour_input, Dice *dice_input);
    // functions for getting player status
    Colour get_Colour() const;
    int get_points() const;
    int get_total_resource() const;
    int get_resource_count(int rec) const;
    std::vector<std::size_t> get_roads() const;
    std::unordered_map<std::size_t, Building_Type> get_buildings() const;

    void print_status() const;
    void print_drc() const;
    void update_player_by_file(std::istringstream &f);
    void output_status_to_file(std::ofstream &f) const;
    void print_buildings() const;
    // functions for purchasing things, they only affect the resources of the player
    // don't have any affect on the actual board
    // purhcase functions assume player has resources to purchase the corresponding item
    bool can_buy_road() const;
    bool can_buy_basement() const;
    bool can_buy_house() const;
    bool can_buy_tower() const;
    bool can_buy_drc() const;

    void purchase_road();
    void purchase_basement();
    void purchase_house();
    void purchase_tower();
    void purchase_drc(std::mt19937 &game_gen);

    // has drcs
    bool has_drc(int index) const;
    void increment_drc(int index, int val);

    // functions for changing player status
    void increment_points();
    void increment_resource(int index, int amount);
    void add_road(size_t edge_id);
    void add_building(size_t node_id, Building_Type building_type);
    // goose and trading functions
    void lose_resource_to_geese(std::default_random_engine &game_rng);
    void steal(Player *victim, std::mt19937 &game_gen);
    void robbed(Player *robber, std::mt19937 &game_gen);
    void trade_resources(Player *other, int num_give, Resource resource_to_give, int num_gain, Resource resource_to_gain);
    // checks if player possesses the resource they are offering (can they make the trade?)
    bool valid_trade_offer(int num_give, Resource resource_to_give) const;
    // checks if player possesses the resource being asked for (can they accept the trade?)
    bool valid_trade_acceptance(int num_gain, Resource resource_to_gain) const;

    // dice functions
    int roll_dice(std::mt19937 &game_gen);
    void setDice(Dice *new_dice);
    Dice *getDice() const;

    bool win() const;
    ~Player();
};

#endif
