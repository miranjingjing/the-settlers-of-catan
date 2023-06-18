#include "player.h"
#include "observer.h"
#include "building.h"

Building::Building(Player *owner): owner{owner} {}

Player* Building::get_Owner() const {
    return owner;
}

Building_Type Building::get_type() const {
    return Building_Type(resource_gain);
}

int Building::get_resource_gain() const {
    return resource_gain;
}

void Building::notify(Subject &s) {
    //increment owner's resource based on the subject's (tile) resource and this's resource_gain amount
    int index = static_cast<int>(s.get_resource());
    owner->increment_resource(index, resource_gain);
}

void Building::upgrade() {
    resource_gain++;
    owner->increment_points();
}

Building::~Building() {
    // don't delete the owner, this is handled in main
}
