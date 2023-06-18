#ifndef _DICE_H_
#define _DICE_H_

#include <iostream>
#include <string>
#include <cstdlib>

//for random
#include <chrono>
#include <random>
#include <algorithm>

class Dice {

    public:
        virtual int generateNumber(std::mt19937 &game_gen) = 0;
        virtual ~Dice();
};

class RandomDice : public Dice {
    
    public:
        int generateNumber(std::mt19937 &game_gen) override;
        ~RandomDice();
};

class LoadedDice : public Dice {
    public:
        int generateNumber(std::mt19937 &game_gen) override;
        ~LoadedDice();
};

#endif
