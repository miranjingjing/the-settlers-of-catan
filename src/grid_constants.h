#ifndef _GRID_CONSTANTS_H_
#define _GRID_CONSTANTS_H_

#include <unordered_map>
#include <vector>
#include <utility>
#include "building.h"

void edge_colour_init(std::unordered_map<size_t, Colour> &map);
void node_owner_init(std::unordered_map<size_t, Building*> &map);
void adjacent_edges_init(std::unordered_map<size_t, std::vector<size_t>> &map);
void edge_ends_init(std::unordered_map<size_t, std::pair<size_t, size_t>> &map);
void adjacent_tiles_init(std::unordered_map<size_t, std::vector<size_t>> &map);

#endif
