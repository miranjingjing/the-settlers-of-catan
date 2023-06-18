#ifndef _TILE_H_
#define _TILE_H_
#include "observer.h"

class Tile : public Subject {
    const Resource RESOURCE;
    const int DICE_VALUE;
    bool hasGoose;

    public:
        Tile(Resource r, int val, bool goose);
        Resource get_resource() const override;
        Resource set_resource();
        int get_dice() const;
        bool getGooseStatus() const;
        void setGooseStatus(bool hasGoose);
        ~Tile();
};

#endif
