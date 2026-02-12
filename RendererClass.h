#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"VBO.h"
#include"VAO.h"
#include"EBO.h"
#include<vector>
#include"shaderClass.h"
#include <glm/glm.hpp>
#include "window.h"
#include "PackedVector.h"


class Renderer
{
public:
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indicies;

	int cubeSize = 1;

	GLfloat angle = 0.0f;

	VAO VAO1;
	VBO VBO1;
	EBO EBO1;

	Renderer();
	~Renderer();
	void clrIndices();
	void clrVertices();
	//void renderCubes(std::vector<Cell*>& ptrList, Cell(&cubeList)[30][20][30]);
	//some veird shit is going on here with pasing a pointer to a vectoer full of pointers pointing to a vec3 inside the array that is passed by refrence instead of full variable 

	//void renderCubeFace(glm::vec3 pos, Direction3D dir);
	void renderMesh(std::vector<PackedVector> data, glm::i32vec3 chunkPos);
	void uploadMesh();
	void allocateMem(size_t sizeV, size_t sizeI);
	void render();
	void handlePrespective(Window* window, Shader& shaderProgram);

};
#endif