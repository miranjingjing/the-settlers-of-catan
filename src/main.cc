#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <exception>

//for random
#include <chrono>
#include <random>

#include "building.h"
#include "grid.h"
#include "observer.h"
#include "player.h"
#include "game.h"

int main(int argc, char **argv) {
    std::string board_file_name = "";
    std::string game_file_name = "";
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    
    bool random_board = false;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-board") {
            i++;
            board_file_name = argv[i];
        } else if (std::string(argv[i]) == "-seed") {
            i++;
            seed = std::stoi(std::string{argv[i]});
        } else if (std::string(argv[i]) == "-load") {
            i++;
            game_file_name = argv[i];
        } else if (std::string(argv[i]) == "-random-board") {
            random_board = true;
        }
        
    }
    std::mt19937 gen(seed);
    std::default_random_engine rng(seed);
    Game *game;
    std::string play_again_cmd;
    bool play_again = true;
    std::cin.exceptions(std::ios::eofbit|std::ios::failbit);
    try {
        if (game_file_name != "") {
            bool start_from_saved_state = true;
            while (play_again) {
                if (start_from_saved_state) {
                    std::ifstream infile{game_file_name};
                    game = new Game(gen, rng, infile, false);
                    game->play(false);
                } else {
                    game = new Game(gen, rng);
                    game->play(false);
                }
                std::cout<<"Would you like to play again?"<<std::endl;
                std::cout<<"> ";
                std::cin>>play_again_cmd;
                if (play_again_cmd != "yes") {
                    // not playing another game, game is deleted at end of program
                    play_again = false;
                } else {
                    delete game;
                    std::cout<<"Would you like to restart from the saved state or from a new random board?"<<std::endl;
                    std::cout<<"Enter 0 to restart from saved state, or 1 to start from a new random board."<<std::endl;
                    std::cout<<"> ";
                    std::string next_game_cmd;
                    std::cin>>next_game_cmd;
                    if (next_game_cmd == "0") {
                        start_from_saved_state = true;
                    } else if (next_game_cmd == "1") {
                        start_from_saved_state = false;
                    }
                } 
            }
        } else if (board_file_name != "") {
            while (play_again) {
                std::ifstream infile{board_file_name};
                game = new Game(gen, rng,infile, true);
                game->play(true);
                std::cout<<"Would you like to play again?"<<std::endl;
                std::cout << "> ";
                std::cin>>play_again_cmd;
                if (play_again_cmd != "yes") {
                    // not playing another game, game is deleted at end of program
                    play_again = false;
                } else {
                    delete game;
                }
            }
            
        } else if (random_board) {
            while (play_again) {
                game = new Game(gen, rng);
                game->play(true);
                std::cout<<"Would you like to play again?"<<std::endl;
                std::cout<<"> ";
                std::cin>>play_again_cmd;
                if (play_again_cmd != "yes") {
                    // not playing another game, game is deleted at end of program
                    play_again = false;
                } else {
                    delete game;
                }
                
            }
        } else {
            while (play_again) {
                std::ifstream infile{"layout.txt"};
                game = new Game(gen, rng, infile, true);
                game->play(true);
                std::cout<<"Would you like to play again?"<<std::endl;
                std::cout<<"> ";
                std::cin>>play_again_cmd;
                if (play_again_cmd != "yes") {
                    // not playing another game, game is deleted at end of program
                    play_again = false;
                } else {
                    delete game;
                }
                
            }
        }
    }
    catch (std::ios::failure &) {
        std::string file_name = "backup.sv";
        std::ofstream file_stream{file_name};
        game->save_game(file_stream);
    }
    delete game;
}
