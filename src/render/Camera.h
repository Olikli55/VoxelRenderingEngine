#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_access.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include"window.h"
#include"shaderClass.h"


class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	int width;
	int height;

	float speed = 0.01f;
	float rotationSpeed = 0.01f;
	float sensitivity = 100.0f;

	// Mouse state
	bool firstClick = true;

	Camera(int w, int h, glm::vec3 pos);

	void Matrix(float fov, float neer, float far, Shader& shader, const char* uniform);
	void Inputs(Window* win);
};

#endif