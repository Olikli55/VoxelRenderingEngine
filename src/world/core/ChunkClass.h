#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

#include "RendererClass.h"
#include <glm/vec3.hpp>
#include <prelin/PerlinNoise.hpp>
#include <vector>
#include "Direction.h"
#include "PackedVector.h"
#include "WorldClass.h"

class World; // Forward declaration

class Chunk {
public:
	GLuint VAO, VBO, EBO;
	glm::i32vec3 position;
	std::vector<PackedVector> mesh;
	static constexpr int CHUNK_SIZE = 32; // static?????????? why????
	bool data[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE] = {};
	World* worldPtr;
	
	Chunk(glm::i32vec3 BasePos, const siv::PerlinNoise& Noise, World* myWorld);
	
	void generateMesh();

	bool isDirty = false; // true = needs re-upload to GPU
private:
	bool isSolid(int x, int y, int z) const;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	void addIndices();
	void allocateMem(size_t sizeV, size_t sizeI); //unsigned long
};

#endif