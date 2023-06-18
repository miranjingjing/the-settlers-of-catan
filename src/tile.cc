#include "tile.h"

Tile::Tile(Resource r, int val, bool goose): RESOURCE{r}, DICE_VALUE{val}, hasGoose{goose} {}

Resource Tile::get_resource() const{
    return RESOURCE;
}

int Tile::get_dice() const {
    return DICE_VALUE;
}

bool Tile::getGooseStatus() const {
    return hasGoose;
}

void Tile::setGooseStatus(bool hasGoose){
    this->hasGoose = hasGoose;
}

Tile::~Tile() {}
