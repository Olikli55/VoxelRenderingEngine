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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "window.h"
#include "Direction.h"
#include "CellClass.h"
#include "PackedVector.h"
#include <glm/vec3.hpp>
#include <unordered_map>

class Renderer
{
public:

	int cubeSize = 1;

	GLfloat angle = 0.0f;

	size_t getSizeIndices() const; // just returning a value. No value will be modfied
	size_t getSizeVertices() const;

	Renderer();
	~Renderer();
	void clrIndices();
	void clrVertices();
	//void renderCubes(std::vector<Cell*>& ptrList, Cell(&cubeList)[30][20][30]);
	//some weird shit is going on here with passing a pointer to a vector full of pointers pointing to a vec3 inside the array that is passed by refrence instead of full variable

	//void renderCubeFace(glm::vec3 pos, Direction3D dir);
	void renderMesh(std::vector<PackedVector> data, glm::i32vec3 chunkPos);
	void uploadMesh();
	void allocateMem(size_t sizeV, size_t sizeI);
	void render();
	void handlePrespective(Window* window, Shader& shaderProgram);

private:
	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

};
#endif