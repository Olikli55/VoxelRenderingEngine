#ifndef DIRECTION_HEADER_H
#define DIRECTION_HEADER_H

#include <glm/glm.hpp>

enum Direction3D {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD
};
// Map enum to vector offsets (x,y)
const glm::ivec3 directionOffsets[] = {
    glm::ivec3(0, 1, 0),    // UP
    glm::ivec3(0, -1, 0),   // DOWN
    glm::ivec3(-1, 0, 0),   // LEFT
    glm::ivec3(1, 0, 0),    // RIGHT
    glm::ivec3(0, 0, 1),    // FORWARD
    glm::ivec3(0, 0, -1)    // BACKWARD
};
#endif