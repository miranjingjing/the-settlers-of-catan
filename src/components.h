#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_
#include <iostream>

enum class Resource {Brick, Energy, Glass, Heat, Wifi, Park};

enum class Building_Type{NoBuilding, Basement, House, Tower};

enum class Colour{NoColour, Blue, Red, Orange, Yellow};

std::ostream &operator<<(std::ostream &out, const Resource r);

std::ostream &operator<<(std::ostream &out, const Building_Type b);

std::ostream &operator<<(std::ostream &out, const Colour c);

const std::string BLUE_OUTPUT =  "\033[34m";
const std::string RED_OUTPUT =  "\033[38;5;196m";
const std::string YELLOW_OUTPUT =  "\033[33m";
const std::string ORANGE_OUTPUT =  "\033[38;5;209m";
const std::string RESET_OUTPUT = "\033[0m";

#endif
