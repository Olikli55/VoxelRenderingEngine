#include "Camera.h"

//constructor
Camera::Camera(int w, int h, glm::vec3 pos){
	width = w;
	height = h;
	Position = pos;
}


void Camera::Matrix(float fov, float neer, float far, Shader& shader, const char* uniform){
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(fov), (float)(width) / (float)(height), neer, far);


	//upload the data to the shaders
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(Window* window)
{
	//running
	if (glfwGetKey(window->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 4.0f;
	}
	else {
		speed = 1.0f;
	}

	//movement controls
	if (glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up)); // cross is used to get the right angle something like that 
	}
	if (glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window->window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}

	//camera rotation
	if (glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		//hide cursor
		glfwSetInputMode(window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//get mouse position
		double mouseX, mouseY;
		glfwGetCursorPos(window->window, &mouseX, &mouseY);

		//prevent camera jump on first click
		if (firstClick)
		{
			glfwSetCursorPos(window->window, (width / 2.0), (height / 2.0));
			firstClick = false;
		}

		// calculate rotation based on mouse offset from center
		float rotX = sensitivity * (float)(mouseY - (height / 2.0)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2.0)) / width;

		// rotate camera vertically pitch around the right vector or left (doesnt matter)
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		//Prevent camera from flipping upside down (limit pitch)
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotate camera horizontally (yaw) around the Up vector
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Reset mouse to center of screen
		glfwSetCursorPos(window->window, (width / 2.0), (height / 2.0));
	}
	else if (glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Show cursor when not clicking
		glfwSetInputMode(window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}

	// Normalize Orientation to prevent drift
	Orientation = glm::normalize(Orientation);
}
