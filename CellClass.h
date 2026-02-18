#ifndef CELL_CLASS_H
#define CELL_CLASS_H

#include <glm/vec3.hpp>

enum Type
{
	AIR,
	STONE,
};
struct Cell
{
	glm::vec3 pos;
	Type type;
};




#endif

