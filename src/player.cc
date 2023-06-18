#include "player.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "resource_costs.h"

std::string Player::print_resource(size_t type) const {
    switch (type) {
        case 0:
            return "Brick";
            break;
        case 1:
            return "Energy";
            break;
        case 2:
            return "Glass";
            break;
        case 3:
            return "Heat";
            break;
        default:
            return "Wifi";
    }
}

Player::Player(Colour colour_input, Dice *dice_input): colour{colour_input}, dice{dice_input} {}

Colour Player::get_Colour() const { return colour; }

int Player::get_points() const { return victory_points; }

// get total number of resources
int Player::get_total_resource() const {
    int count = 0;
    for (auto r : resource_count) {
        count += r;
    }
    return count;
}

std::vector<std::size_t> Player::get_roads() const { return roads; }

std::unordered_map<std::size_t, Building_Type> Player::get_buildings() const { return buildings; }

void Player::print_status() const {
    std::cout << this->get_Colour() << " has " << victory_points << " building points, ";
    for (int i = 0; i < 5; i++) {
        std::cout << resource_count.at(i) << " " << print_resource(i) << ((i == 4) ? "." : ", ");
    }
    std::cout << std::endl;
}

void Player::update_player_by_file(std::istringstream &s) {
    for (int i = 0; i < 5; i++) {
        s >> resource_count[i];
    }
    char input;
    int road_num;

    s >> input;  // read r
    while (s >> road_num) {
        roads.emplace_back(static_cast<size_t>(road_num));
    }
    // if break out of loop we have read in h
    s.clear();
    s.ignore();

    int building_num;
    char building_type_input;
    Building_Type building_type;
    while (s >> building_num) {
        s >> building_type_input;
        if (building_type_input == 'B') {
            building_type = Building_Type::Basement;
        } else if (building_type_input == 'H') {
            building_type = Building_Type::House;
        } else if (building_type_input == 'T') {
            building_type = Building_Type::Tower;
        }
        buildings[building_num] = building_type;
    }
    // terminate loop when reach end of file for stringstream
}

void Player::output_status_to_file(std::ofstream &f) const {
    // output resources
    for (auto x : resource_count) {
        f << x << " ";
    }
    // output roads
    f << "r ";
    for (auto x : roads) {
        f << x << " ";
    }
    // output buildings
    f << "h ";
    for (auto kv : buildings) {
        f << kv.first << " " << kv.second << " ";
    }
    f << std::endl;
}

void Player::print_buildings() const {
    std::cout << this->get_Colour() << " has built:" << std::endl;
    for (auto kv : buildings) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }
}

bool Player::can_buy_road() const {
    for (size_t i = 0; i < resource_count.size(); i++) {
        if (resource_count.at(i) < road_cost.at(i)) {
            return false;
        }
    }
    return true;
}
bool Player::can_buy_basement() const {
    for (size_t i = 0; i < resource_count.size(); i++) {
        if (resource_count.at(i) < basement_cost.at(i)) {
            return false;
        }
    }
    return true;
}
bool Player::can_buy_house() const {
    for (size_t i = 0; i < resource_count.size(); i++) {
        if (resource_count.at(i) < house_cost.at(i)) {
            return false;
        }
    }
    return true;
}
bool Player::can_buy_tower() const {
    for (size_t i = 0; i < resource_count.size(); i++) {
        if (resource_count.at(i) < tower_cost.at(i)) {
            return false;
        }
    }
    return true;
}

void Player::purchase_road() {
    for (size_t i = 0; i < resource_count.size(); i++) {
        resource_count.at(i) -= road_cost.at(i);
    }
}
void Player::purchase_basement() {
    for (size_t i = 0; i < resource_count.size(); i++) {
        resource_count.at(i) -= basement_cost.at(i);
    }
}
void Player::purchase_house() {
    for (size_t i = 0; i < resource_count.size(); i++) {
        resource_count.at(i) -= house_cost.at(i);
    }
}
void Player::purchase_tower() {
    for (size_t i = 0; i < resource_count.size(); i++) {
        resource_count.at(i) -= tower_cost.at(i);
    }
}

void Player::increment_points() { this->victory_points++; }

void Player::increment_resource(int index, int amount) {
    this->resource_count[index] += amount;
}

void Player::add_road(size_t edge_id) {
    // only add if edge_id doesn't already exist
    // we need to check this for when we create a game from a loaded state
    bool exist_already = false;
    for (auto x : roads) {
        if (x == edge_id) {
            exist_already = true;
        }
    }
    if (!exist_already) {
        roads.emplace_back(edge_id);
    }
}

void Player::add_building(size_t node_id, Building_Type building_type) {
    buildings[node_id] = building_type;
}

void Player::lose_resource_to_geese(std::default_random_engine &game_rng) {
    int total_resources_count = get_total_resource();

    if (total_resources_count >= 10) {
        int half = total_resources_count / 2;

        std::cout << "Builder " << this->get_Colour() << " loses " << half << " resources to the geese. They lose:" << std::endl;

        std::vector<int> resource_lost_count{
            0, 0, 0, 0, 0};  // purely used to keep track of cumulative amount
                             // of each resource lost
        std::vector<int> resource_pool;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < this->resource_count[i]; j++)
                resource_pool.push_back(i);
        std::shuffle(resource_pool.begin(), resource_pool.end(), game_rng);

        for (int i = 0; i < half; i++) {
            resource_lost_count[resource_pool[i]]++;
            this->resource_count[resource_pool[i]]--;
        }

        for (int i = 0; i < 5; i++)
            std::cout << resource_lost_count[i] << " " << print_resource(i)
                      << ((i == 4) ? "." : ", ");
        std::cout << std::endl;
    }
}
// steals a resource from a player assuming they are valid (they have at least
// one resource)
void Player::steal(Player *victim, std::mt19937 &game_gen) { victim->robbed(this, game_gen); }

void Player::robbed(Player *robber, std::mt19937 &game_gen) {
    std::uniform_int_distribution<std::mt19937::result_type> dist4(
        1, this->get_total_resource());
    int resource = dist4(game_gen);

    for (int i = 0; i < 5; i++) {
        resource -= this->resource_count[i];
        if (resource <= 0) {
            resource_count[i]--;
            robber->increment_resource(i, 1);
            std::cout << "Builder " << robber->get_Colour() << " steals " << print_resource(i) << " from builder " << this->get_Colour() << "." << std::endl;
            return;
        }
    }
}

void Player::trade_resources(Player *other, int num_give, Resource resource_to_give,
                             int num_gain, Resource resource_to_gain) {
    int give_index = static_cast<int>(resource_to_give);
    int gain_index = static_cast<int>(resource_to_gain);

    this->resource_count[give_index] -= num_give;  // trade resources
    other->resource_count[give_index] += num_give;
    this->resource_count[gain_index] += num_gain;
    other->resource_count[gain_index] -= num_gain;
}

bool Player::valid_trade_offer(int num_give, Resource resource_to_give) const {
    int give_index = static_cast<int>(resource_to_give);
    if (resource_count.at(give_index) < num_give) return false;
    return true;
}

bool Player::valid_trade_acceptance(int num_gain, Resource resource_to_gain) const {
    int gain_index = static_cast<int>(resource_to_gain);
    if (resource_count.at(gain_index) < num_gain) return false;
    return true;
}

int Player::roll_dice(std::mt19937 &game_gen) { return dice->generateNumber(game_gen); }

void Player::setDice(Dice *new_dice) { dice = new_dice; }

Dice *Player::getDice() const { return dice; }

bool Player::win() const {
    if (victory_points >= 10) {
        return true;
    }
    return false;
}

Player::~Player() {}

bool Player::can_buy_drc() const {
    for (size_t i = 0; i < resource_count.size(); i++) {
        if (resource_count.at(i) < drc_cost.at(i)) {
            return false;
        }
    }
    return true;
}

void Player::purchase_drc(std::mt19937 & game_gen) {
    for (size_t i = 0; i < resource_count.size(); i++) {
        resource_count.at(i) -= drc_cost.at(i);
    }
    std::uniform_int_distribution<std::mt19937::result_type> dist4(0, 4);
    int drc = dist4(game_gen);
    this->drc_count[drc]++;

    const std::vector<std::string> stmt = {"knight", "Year of plenty", "monopoly", "victory point", "road building"};
    std::cout << "Congrats! You have bought a " << stmt[drc] << ". ";
}

void Player::print_drc() const {
    const std::vector<std::string> stmt = {"Knight", "Year of plenty", "Monopoly", "Victory point", "Road building"};

    std::cout << this->get_Colour() << " has the following:" << std::endl;
    for (int i = 0; i < 5; i++) {
        std::cout << stmt.at(i) << ": " << drc_count.at(i) << std::endl;
    }
    std::cout << std::endl;
}

bool Player::has_drc(int index) const{
    return this->drc_count.at(index) > 0;
}

int Player::get_resource_count(int rec) const{
    return this->resource_count.at(rec);
}

void Player::increment_drc(int index, int val){
    this->drc_count[index] += val;
}
