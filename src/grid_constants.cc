#include "grid_constants.h"

const int MAX_EDGE = 71;
const int MAX_NODE = 53;

using size_t = std::size_t;

void edge_colour_init(std::unordered_map<size_t, Colour> &map) {
    for (size_t i = 0; i <= MAX_EDGE; i++) {
        map[i] = Colour::NoColour;
    }
}

void node_owner_init(std::unordered_map<size_t, Building *> &map) {
    for (size_t i = 0; i <= MAX_NODE; i++) {
        map[i] = nullptr;
    }
}

void edge_ends_init(std::unordered_map<size_t, std::pair<size_t, size_t>> &map) {
    map[0] = std::make_pair(0, 1);
    map[1] = std::make_pair(0, 3);
    map[2] = std::make_pair(1, 4);
    map[3] = std::make_pair(2, 3);
    map[4] = std::make_pair(4, 5);
    map[5] = std::make_pair(2, 7);
    map[6] = std::make_pair(3, 8);
    map[7] = std::make_pair(4, 9);
    map[8] = std::make_pair(5, 10);
    map[9] = std::make_pair(6, 7);
    map[10] = std::make_pair(8, 9);
    map[11] = std::make_pair(10, 11);
    for (size_t i = 12; i <= 17; i++) {
        map[i] = std::make_pair(i - 6, i);
    }
    map[18] = std::make_pair(13, 14);
    map[19] = std::make_pair(15, 16);
    for (size_t i = 20; i <= 25; i++) {
        map[i] = std::make_pair(i - 8, i - 2);
    }
    map[26] = std::make_pair(18, 19);
    map[27] = std::make_pair(20, 21);
    map[28] = std::make_pair(22, 23);
    for (size_t i = 29; i <= 34; i++) {
        map[i] = std::make_pair(i - 11, i - 5);
    }
    map[35] = std::make_pair(25, 26);
    map[36] = std::make_pair(27, 28);
    for (size_t i = 37; i <= 42; i++) {
        map[i] = std::make_pair(i - 13, i - 7);
    }
    map[43] = std::make_pair(30, 31);
    map[44] = std::make_pair(32, 33);
    map[45] = std::make_pair(34, 35);
    for (size_t i = 46; i <= 51; i++) {
        map[i] = std::make_pair(i - 16, i - 10);
    }
    map[52] = std::make_pair(37, 38);
    map[53] = std::make_pair(39, 40);
    for (size_t i = 54; i <= 59; i++) {
        map[i] = std::make_pair(i - 18, i - 12);
    }
    map[60] = std::make_pair(42, 43);
    map[61] = std::make_pair(44, 45);
    map[62] = std::make_pair(46, 47);
    for (size_t i = 63; i <= 66; i++) {
        map[i] = std::make_pair(i - 20, i - 15);
    }
    map[67] = std::make_pair(48, 49);
    map[68] = std::make_pair(50, 51);
    map[69] = std::make_pair(49, 52);
    map[70] = std::make_pair(50, 53);
    map[71] = std::make_pair(52, 53);
}

void adjacent_edges_init(std::unordered_map<size_t, std::vector<size_t>> &map) {
    map[0] = {0, 1};
    map[1] = {0, 2};
    map[2] = {3, 5};
    map[3] = {1, 3, 6};
    map[4] = {2, 4, 7};
    map[5] = {4, 8};
    map[6] = {9, 12};
    map[7] = {5, 9, 13};
    map[8] = {6, 10, 14};
    map[9] = {7, 10, 15};
    map[10] = {8, 11, 16};
    map[11] = {11, 17};
    map[12] = {12, 20};
    map[13] = {13, 18, 21};
    map[14] = {14, 18, 22};
    map[15] = {15, 19, 23};
    map[16] = {16, 19, 24};
    map[17] = {17, 25};
    map[18] = {20, 26, 29};
    map[19] = {21, 26, 30};
    map[20] = {22, 27, 31};
    map[21] = {23, 27, 32};
    map[22] = {24, 28, 33};
    map[23] = {25, 28, 34};
    map[24] = {29, 37};
    map[25] = {30, 35, 38};
    map[26] = {31, 35, 39};
    map[27] = {32, 36, 40};
    map[28] = {33, 36, 41};
    map[29] = {34, 42};
    map[30] = {37, 43, 46};
    map[31] = {38, 43, 47};
    map[32] = {39, 44, 48};
    map[33] = {40, 44, 49};
    map[34] = {41, 45, 50};
    map[35] = {42, 45, 51};
    map[36] = {46, 54};
    map[37] = {47, 52, 55};
    map[38] = {48, 52, 56};
    map[39] = {49, 53, 57};
    map[40] = {50, 53, 58};
    map[41] = {51, 59};
    map[42] = {54, 60};
    map[43] = {55, 60, 63};
    map[44] = {56, 61, 64};
    map[45] = {57, 61, 65};
    map[46] = {58, 62, 66};
    map[47] = {59, 62};
    map[48] = {63, 67};
    map[49] = {64, 67, 69};
    map[50] = {65, 68, 70};
    map[51] = {66, 68};
    map[52] = {69, 71};
    map[53] = {70, 71};
}

void adjacent_tiles_init(std::unordered_map<size_t, std::vector<size_t>> &map) {
    map[0] = {0};
    map[1] = {0};
    map[2] = {1};
    map[3] = {0,1};
    map[4] = {0,2};
    map[5] = {2};
    map[6] = {3};
    map[7] = {1,3};
    map[8] = {0,1,4};
    map[9] = {0,2,4};
    map[10] = {2,5};
    map[11] = {5};
    map[12] = {3};
    map[13] = {1,3,6};
    map[14] = {1,4,6};
    map[15] = {2,4,7};
    map[16] = {2,5,7};
    map[17] = {5};
    map[18] = {3,8};
    map[19] = {3,6,8};
    map[20] = {4,6,9};
    map[21] = {4,7,9};
    map[22] = {5,7,10};
    map[23] = {5,10};
    map[24] = {8};
    map[25] = {6,8,11};
    map[26] = {6,9,11};
    map[27] = {7,9,12};
    map[28] = {7,10,12};
    map[29] = {10};
    map[30] = {8,13};
    map[31] = {8,11,13};
    map[32] = {9,11,14};
    map[33] = {9,12,14};
    map[34] = {10,12,15};
    map[35] = {10,15};
    map[36] = {13};
    map[37] = {11,13,16};
    map[38] = {11,14,16};
    map[39] = {12,14,17};
    map[40] = {12,15,17};
    map[41] = {15};
    map[42] = {13};
    map[43] = {13,16};
    map[44] = {14,16,18};
    map[45] = {14,17,18};
    map[46] = {15,17};
    map[47] = {15};
    map[48] = {16};
    map[49] = {16,18};
    map[50] = {17,18};
    map[51] = {17};
    map[52] = {18};
    map[53] = {18};
} 
