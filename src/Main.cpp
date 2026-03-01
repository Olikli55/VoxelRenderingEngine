#include<iostream>
#include "glad/glad.h"	
#include<GLFW/glfw3.h>
#include <cmath>
#include "shaderClass.h"
#include "window.h"
#include "RendererClass.h"
#include <glm/glm.hpp>
#include "Camera.h"
#include"Texture.h"
#include "WorldClass.h"
#include <thread>
#include <chrono>  



int main() {


	glm::i32vec3 previusChunk;
	glm::i32vec3 currentChunk;
	World myWorld;
	Window myWindow(1080, 1920); // init a window
	//Shader shaderProgram("C:/Users/oliver/Documents/GameDev/C-C#-C++/C++/OpenGl/3Dlearning/default.vert", "C:/Users/oliver/Documents/GameDev/C-C#-C++/C++/OpenGl/3Dlearning/default.frag"); // init the sahder
	//Shader shaderProgram("default.vert", "default.frag"); // init the sahder
	Shader shaderProgram("/home/plutonium/CLionProjects/VoxelRenderingEngine/assets/shaders/default.vert",
					 "/home/plutonium/CLionProjects/VoxelRenderingEngine/assets/shaders/default.frag");
	Renderer myRenderer;
	Camera myCamera(myWindow.w, myWindow.h, glm::vec3(0.0f, 100.0f, 0.0f)); 
	//Texture cubeTexture("C:/Users/oliver/Documents/GameDev/C-C#-C++/C++/OpenGl/3Dlearning/texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	//Texture cubeTexture("texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture cubeTexture("/home/plutonium/CLionProjects/VoxelRenderingEngine/assets/textures/texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	cubeTexture.texUnit(shaderProgram, "tex0", 0);

	
	myWorld.generateChunkAround(glm::i32vec3(0,0,0));
    myWorld.buildAndUploadMesh(myRenderer);

	while (!glfwWindowShouldClose(myWindow.window)) {

		currentChunk = glm::i32vec3(
			std::floor(myCamera.Position.x / 32),
			0,
			std::floor(myCamera.Position.z / 32)
		);
		shaderProgram.Activate();

		myCamera.Matrix(45.0f, 0.1f, 10000.0f, shaderProgram, "camMatrix");
		myCamera.Inputs(&myWindow);

		cubeTexture.Bind();

		myRenderer.render();
		glfwSwapBuffers(myWindow.window);      // Swap the back buffer with the front buffer
		glfwPollEvents();


		if (currentChunk != previusChunk) {

			myWorld.generateChunkAround(currentChunk);


			myWorld.AppendUploadMesh(myRenderer);

		}

 



		previusChunk = currentChunk;
	}

	//END======================================END


	shaderProgram.Delete(); 
	cubeTexture.Delete();
	glfwDestroyWindow(myWindow.window); // Delete window before ending the program
	glfwTerminate();
	return 0;
}