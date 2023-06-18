#include "dice.h"

Dice::~Dice() {}

int RandomDice::generateNumber(std::mt19937 &game_gen) {
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6);
    int dice1 = dist6(game_gen);
    int dice2 = dist6(game_gen);
    return dice1 + dice2;
}

RandomDice::~RandomDice() {}

int LoadedDice::generateNumber(std::mt19937 &game_gen) {
    int min = 2, max = 12;
    int roll = 0;
    while (true) {
        std::cout << "Input a roll between " << min << " and " << max << ".\n> "; // prompt for roll
        std::cin >> roll;
        if (roll < min || roll > max) { // not within range
            std::cout << "Invalid roll." << std::endl;
        }
        else break;
    }
    return roll;
}

LoadedDice::~LoadedDice() {}
