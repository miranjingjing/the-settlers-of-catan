#ifndef _GAME_H_
#define _GAME_H_
#include "player.h"
#include "grid.h"
#include <fstream>

class Game {
    std::vector<Player*> players;//blue, red, orange, yellow
    int turn;
    Grid *g; 
    //dice
    Dice *fair;
    Dice *load;
    std::mt19937 game_gen;
    std::default_random_engine game_rng;

    public:
        //constructor for random board
        Game(std::mt19937 gen, std::default_random_engine rng);
        //constructor for when the board is set
        // case 1: new_game = true, the command line argument was strictly -board
        // case 2: new_game = false, the command line argument was -load
        Game(std::mt19937 gen, std::default_random_engine rng, std::ifstream &f, bool new_game);
        
        void save_game(std::ofstream &f);
        void play(bool play_beginning);
        ~Game();
};

#endif
