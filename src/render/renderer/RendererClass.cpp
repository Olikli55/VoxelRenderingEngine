#include "RendererClass.h"
#include "Direction.h"
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <chrono>
#include <iostream>
    Renderer::Renderer()
	    :
	    VAO1(),
	    VBO1(vertices.data(), vertices.size() * sizeof(GLfloat)), 	// Generates Vertex Buffer Object and links it to vertices
	    EBO1(indices.data(), indices.size() * sizeof(GLuint))      // Generates Element Buffer Object and links it to indices


    {
        glfwSwapInterval(0);// disables the v-Sync
	    // Generates Vertex Array Object and binds it
	    VAO1.Bind(); // dont i need to bind other too? strange


	    // Links VBO attributes such as coordinates and colors to VAO with information on how much everything
	    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(GLfloat), (void*)0);
	    VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // chek this!!!!



        EBO1.Bind(); // This links EBO to the currently bound VAO
	    VAO1.Unbind();
	    VBO1.Unbind();
	    EBO1.Unbind();// Unbind all to prevent accidentally modifying them bc "i am stupid" (f1 refrence)

	    glEnable(GL_DEPTH_TEST);

	    // Enable face culling to only render front faces
	   //  glEnable(GL_CULL_FACE);
	   // glCullFace(GL_BACK);      // Cull back faces
	   // glFrontFace(GL_CCW);      // Front faces use counter-clockwise winding
    }




void Renderer::clrIndices() {
	indices.clear();
}

void Renderer::clrVertices() {
	vertices.clear();
}

Renderer::~Renderer() {
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	// deletes vereything so there are no dangling pointers
}

void Renderer::render() {
   // auto start = std::chrono::high_resolution_clock::now();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind buffers, upload current vertex/index data, then draw
    VBO1.Bind();
    VAO1.Bind();
    EBO1.Bind();

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    //10-50ms
   // auto end = std::chrono::high_resolution_clock::now();
   // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
   // std::cout << "Render time: " << duration.count() << " microseconds" << std::endl;
}



void Renderer::handlePrespective(Window* myWindow, Shader& shaderProgram) {

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	angle += 0.01f;

	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)(myWindow->h / myWindow->w), 0.1f, 100.0f);

	int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	int viewlLoc = glGetUniformLocation(shaderProgram.ID, "view");
	glUniformMatrix4fv(viewlLoc, 1, GL_FALSE, glm::value_ptr(view));

	int projectionlLoc = glGetUniformLocation(shaderProgram.ID, "projection");
	glUniformMatrix4fv(projectionlLoc, 1, GL_FALSE, glm::value_ptr(projection));

}


void Renderer::renderMesh(std::vector<PackedVector> data, glm::i32vec3 chunkPos) {
	//we need to convert the local position to global positions by using the chunk pos and also we
	//need to multiply the chunk pos by the size of the chunk (32) and add to it the data than we get int that we can render also in the packed vector thre is a direction the face is facing
	glm::i32vec3 chunkWorldPos = glm::vec3(
		chunkPos.x * 32,  // CHUNK_SIZE = 32
		chunkPos.y * 32,
		chunkPos.z * 32);

	// texture atlas constants: 192x32 with 6 textures of 32x32 each
	const float texWidth = 1.0f / 6.0f;  // each texture occupies 1/6th of the atlas width

	for (const PackedVector& unpackedData : data) {
        // adds the global position of the chunk and local chunk post to get the global position of each cube
		glm::i32vec3 blockWorldPos = chunkWorldPos + glm::i32vec3(unpackedData.x, unpackedData.y, unpackedData.z);
		Direction3D dir = static_cast<Direction3D>(unpackedData.direction); // cojvert the packed direction to unpacked direction

        unsigned int startIndex = static_cast<unsigned int>(vertices.size() / 5); // 5 floats per vertex
        std::vector<GLfloat> tempVertices;

        int fullSize = cubeSize;
        const float texWidth = 32.0f / 256.0f;
        const float texHeight = 32.0f / 256.0f;
        float texLeft;
        float texRight;

        // GLfloat to ensure the float size on all devices
        const float tileSize = 32.0f / 256.0f; // = 0.125

        switch (dir) {
        case UP: {
            float idx = 0.0f;
            float u0 = idx * tileSize, u1 = u0 + tileSize;
            float v0 = 0.0f, v1 = tileSize;
            tempVertices = {
                (GLfloat)blockWorldPos.x,              (GLfloat)blockWorldPos.y, (GLfloat)blockWorldPos.z,              u0, v0,
                (GLfloat)(blockWorldPos.x + fullSize), (GLfloat)blockWorldPos.y, (GLfloat)blockWorldPos.z,              u1, v0,
                (GLfloat)(blockWorldPos.x + fullSize), (GLfloat)blockWorldPos.y, (GLfloat)(blockWorldPos.z - fullSize), u1, v1,
                (GLfloat)blockWorldPos.x,              (GLfloat)blockWorldPos.y, (GLfloat)(blockWorldPos.z - fullSize), u0, v1,
            };
            break;
        }
        case DOWN: {
            float idx = 1.0f;
            float u0 = idx * tileSize, u1 = u0 + tileSize;
            float v0 = 0.0f, v1 = tileSize;
            tempVertices = {
                (GLfloat)blockWorldPos.x,              (GLfloat)(blockWorldPos.y - fullSize), (GLfloat)(blockWorldPos.z - fullSize), u0, v0,
                (GLfloat)(blockWorldPos.x + fullSize), (GLfloat)(blockWorldPos.y - fullSize), (GLfloat)(blockWorldPos.z - fullSize), u1, v0,
                (GLfloat)(blockWorldPos.x + fullSize), (GLfloat)(blockWorldPos.y - fullSize), (GLfloat)blockWorldPos.z,              u1, v1,
                (GLfloat)blockWorldPos.x,              (GLfloat)(blockWorldPos.y - fullSize), (GLfloat)blockWorldPos.z,              u0, v1,
            };
            break;
        }
        case LEFT: {
            float idx = 2.0f;
            float u0 = idx * tileSize, u1 = u0 + tileSize;
            float v0 = 0.0f, v1 = tileSize;
            tempVertices = {
                (GLfloat)blockWorldPos.x, (GLfloat)(blockWorldPos.y - fullSize), (GLfloat)blockWorldPos.z,              u0, v0,
                (GLfloat)blockWorldPos.x, (GLfloat)blockWorldPos.y,              (GLfloat)blockWorldPos.z,              u1, v0,
                (GLfloat)blockWorldPos.x, (GLfloat)blockWorldPos.y,              (GLfloat)(blockWorldPos.z - fullSize), u1, v1,
                (GLfloat)blockWorldPos.x, (GLfloat)(blockWorldPos.y - fullSize), (GLfloat)(blockWorldPos.z - fullSize), u0, v1,
            };
            break;
        }
        case RIGHT: {
            float idx = 3.0f;
            float u0 = idx * tileSize, u1 = u0 + tileSize;
            float v0 = 0.0f, v1 = tileSize;
            tempVertices = {
                (GLfloat)(blockWorldPos.x + fullSize), (GLfloat)(blockWorldPos.y - fullSize), (GLfloat)(blockWorldPos.z - fullSize), u0, v0,
                (GLfloat)(blockWorldPos.x + fullSize), (GLfloat)blockWorldPos.y,              (GLfloat)(blockWorldPos.z - fullSize), u1, v0,
                (GLfloat)(blockWorldPos.x + fullSize), (GLfloat)blockWorldPos.y,              (GLfloat)blockWorldPos.z,              u1, v1,
                (GLfloat)(blockWorldPos.x + fullSize), (GLfloat)(blockWorldPos.y - fullSize), (GLfloat)blockWorldPos.z,              u0, v1,
            };
            break;
        }
        case FORWARD: {
            float idx = 4.0f;
            float u0 = idx * tileSize, u1 = u0 + tileSize;
            float v0 = 0.0f, v1 = tileSize;
            tempVertices = {
                (GLfloat)blockWorldPos.x,              (GLfloat)(blockWorldPos.y - fullSize), (GLfloat)blockWorldPos.z, u0, v0,
                (GLfloat)(blockWorldPos.x + fullSize), (GLfloat)(blockWorldPos.y - fullSize), (GLfloat)blockWorldPos.z, u1, v0,
                (GLfloat)(blockWorldPos.x + fullSize), (GLfloat)blockWorldPos.y,              (GLfloat)blockWorldPos.z, u1, v1,
                (GLfloat)blockWorldPos.x,              (GLfloat)blockWorldPos.y,              (GLfloat)blockWorldPos.z, u0, v1,
            };
            break;
        }
        case BACKWARD: {
            float idx = 5.0f;
            float u0 = idx * tileSize, u1 = u0 + tileSize;
            float v0 = 0.0f, v1 = tileSize;
            tempVertices = {
                (GLfloat)(blockWorldPos.x + fullSize), (GLfloat)(blockWorldPos.y - fullSize), (GLfloat)(blockWorldPos.z - fullSize), u0, v0,
                (GLfloat)blockWorldPos.x,              (GLfloat)(blockWorldPos.y - fullSize), (GLfloat)(blockWorldPos.z - fullSize), u1, v0,
                (GLfloat)blockWorldPos.x,              (GLfloat)blockWorldPos.y,              (GLfloat)(blockWorldPos.z - fullSize), u1, v1,
                (GLfloat)(blockWorldPos.x + fullSize), (GLfloat)blockWorldPos.y,              (GLfloat)(blockWorldPos.z - fullSize), u0, v1,
            };
            break;
        }
        }
        // create indices for this face (2 triangles = 6 indices)
        std::vector<GLuint> tempIndices = {
            startIndex + 0, startIndex + 1, startIndex + 2,
            startIndex + 0, startIndex + 2, startIndex + 3,
        };

        // Add to main vertex and index buffers
        vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
        indices.insert(indices.end(), tempIndices.begin(), tempIndices.end());
    }

}

void Renderer::uploadMesh() {
    VBO1.Bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
    EBO1.Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);
}


void Renderer::allocateMem(size_t sizeV, size_t sizeI) {

    //   only reserve if it needs more capacity than it has
    if (indices.capacity() < sizeI) {
        indices.reserve(sizeI);
    }

    if (vertices.capacity() < sizeV) {
        vertices.reserve(sizeV);
    }

}


size_t Renderer::getSizeIndices() const
{
	return indices.size();
    }

size_t Renderer::getSizeVertices() const
{
	    return vertices.size();
    }
