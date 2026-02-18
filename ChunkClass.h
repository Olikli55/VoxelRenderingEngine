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
	inline static constexpr int CHUNK_SIZE = 32; // no FING IDEA WHAT IT DOES 
	bool data[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE] = {0};
	World* worldPtr;
	
	Chunk(glm::i32vec3 BasePos, const siv::PerlinNoise& Noise, World* myWorld);
	
	void generateMesh();
	void scaleDown();
	void scaleToNormal();
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	bool isDirty = false; // true = needs re-upload to GPU
private:
	bool isSolid(int x, int y, int z) const;	
};

#endif