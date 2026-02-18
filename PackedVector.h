#ifndef PACKED_VECTOR_H
#define PACKED_VECTOR_H


struct PackedVector
{
	unsigned int x : 6;
	unsigned int y : 6;
	unsigned int z : 6;
	unsigned int direction : 3;
	
	//constructor
	PackedVector(unsigned int _x, unsigned int _y, unsigned int _z, unsigned int _dir)
		: x(_x), y(_y), z(_z), direction(_dir) {}
		
	//empty overloaded constructor
	PackedVector() : x(0), y(0), z(0), direction(0) {}
};


#endif