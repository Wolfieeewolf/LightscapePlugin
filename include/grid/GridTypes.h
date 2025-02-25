#pragma once

#include <QtCore>
#include <functional>

struct GridPosition {
    int x;
    int y;
    int z;
    
    GridPosition(int _x = 0, int _y = 0, int _z = 0) 
        : x(_x), y(_y), z(_z) {}
        
    bool operator==(const GridPosition& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator<(const GridPosition& other) const {
        if (z != other.z) return z < other.z;
        if (y != other.y) return y < other.y;
        return x < other.x;
    }
};

// Hash function for GridPosition
inline uint qHash(const GridPosition& pos, uint seed = 0) noexcept
{
    return qHash(pos.x, seed) ^ qHash(pos.y, seed) ^ qHash(pos.z, seed);
}