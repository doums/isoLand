

#ifndef ISOLAND_DIRECTION_H
#define ISOLAND_DIRECTION_H

enum class Direction : int {
    North = 0,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest
};

inline Direction operator++(Direction& dir, int) {
    if (dir == Direction::NorthWest)
        return dir = Direction::North;
    else {
        using IntType = typename std::underlying_type<Direction >::type;
        return (dir = static_cast<Direction>( static_cast<IntType>(dir) + 1 ));
    }
}

inline Direction operator--(Direction& dir, int) {
    if (dir == Direction::North)
        return dir = Direction::NorthWest;
    else {
        using IntType = typename std::underlying_type<Direction >::type;
        return (dir = static_cast<Direction>( static_cast<IntType>(dir) - 1 ));
    }
}

#endif //ISOLAND_DIRECTION_H
