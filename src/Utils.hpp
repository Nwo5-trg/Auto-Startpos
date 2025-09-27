#pragma once

// settings stuff is a lil much to include in a single file but if im gonna have an .hpp might as well also keep the maps in there so

namespace Utils {
    inline std::unordered_map<int, int> objToEntry {
        {12, 0}, {13, 0}, {47, 0}, {111, 0}, {660, 0}, {745, 0}, {1331, 0}, {1933, 0},
        {200, 1}, {201, 1}, {202, 1}, {203, 1}, {1334, 1},
        {10, 2}, {11, 2},
        {286, 3}, {287, 3},
        {99, 4}, {101, 4},
        {45, 5}, {46, 5},
        {67, 6}, {3004, 6}, {3005, 6},
    };
    
    inline std::unordered_map<int, int> portalToGamemode = {
        {12, 0}, {13, 1}, {47, 2}, {111, 3}, {660, 4}, 
        {745, 5}, {1331, 6}, {1933, 7},
    };

    inline std::unordered_map<int, Speed> portalToSpeed = {
        {200, Speed::Slow}, {201, Speed::Normal}, {202, Speed::Fast}, 
        {203, Speed::Faster}, {1334, Speed::Fastest}
    };

    bool getPadDirection(float rotation);
}

namespace Settings {
    inline bool advancedGravity;
    inline std::array<bool, 6> enabledModes;

    void updateSettings();
}