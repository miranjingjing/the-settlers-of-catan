#ifndef _GRID_H_
#define _GRID_H_
#include <vector>
#include <utility>
#include <unordered_map>
#include <iostream>
#include "building.h"
#include "tile.h"


class Grid {
    using size_t = std::size_t;
    const size_t MAX_EDGE = 71;
    const size_t MAX_NODE = 53;
    const size_t MAX_TILE = 18;

    // for each node -> list of adjacent edges
    std::unordered_map<size_t, std::vector<size_t>> adjacent_edges;
    // for each edge -> gives the pair of nodes 
    std::unordered_map<size_t, std::pair<size_t, size_t>> edge_ends;
    // which roads are owned by who
    std::unordered_map<size_t, Colour> edge_colour;
    // which vertices are owned by "who" -> this information is in the Building
    std::unordered_map<size_t, Building *> node_owner;
    // for each node -> list of adjacent tiles (tiles it earns income from)
    std::unordered_map<size_t, std::vector<size_t>> adjacent_tiles;

    std::vector<Tile *> tiles;

    size_t goose_tile;

    // print helper methods
    void print_edge(size_t &) const;
    void print_node(size_t &) const;
    void print_tile_break(int n) const;
    void print_resource(Resource r) const;
    void print_tile_edge(size_t &, size_t &) const;
    void print_tile_num(size_t & counter) const;
    void print_tile_dice(size_t & counter) const;
    void print_tile_res(size_t & counter) const;    
    void print_possible_goose(size_t & counter) const;

    public: 
        // constructor for random board generation
        Grid(std::default_random_engine& rng);
        // constructor for file input board generation
        Grid(std::istringstream &f);
        
        // getters and setters
        Building_Type get_building_type_at_node(size_t node_id) const;
        size_t get_goose_tile() const;
        void set_goose(size_t new_tile);

        void save_board(std::ofstream& f) const;

        // can player PLACE a road here
        bool valid_road(Colour player, size_t edge_id) const;
        // can player PLACE a building here
        bool valid_building(Colour player, size_t node_id, bool starting_buildings) const;
        // check if you can upgrade your building -> according to rules of the game
        bool valid_upgrade(Colour colour, size_t node_id) const; 
        void build_road(Player* player, size_t edge_id);
        void build_building(Player *player, size_t node_id);
        void upgrade_building(Player *player, size_t node_id);

        void update_by_roll(int roll);
        
        size_t move_goose(); 
        int who_to_steal_from(size_t geese_loc, Player* curr_player);

        void print_grid() const;
        
        ~Grid();
};

#endif
