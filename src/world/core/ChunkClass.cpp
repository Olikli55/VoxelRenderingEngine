#include"ChunkClass.h"
#include <glm/vec3.hpp>
#include "Direction.h"

    Chunk::Chunk(glm::i32vec3 BasePos, const siv::PerlinNoise& Noise, World* myWorld)
    : vao()
    , vbo(vertices.data(), vertices.size() * sizeof(GLfloat))
    , ebo(indices.data(), indices.size() * sizeof(GLuint))
    , position(BasePos)
    , worldPtr(myWorld)


{
        setupBuffers();
    for (unsigned short x = 0; x < CHUNK_SIZE; x++)
    {
        for (unsigned short z = 0; z < CHUNK_SIZE; z++)
        {

            // Calculate world coordinates for the column
            const double worldX = static_cast<double>(x) + position.x * CHUNK_SIZE;
            const double worldZ = static_cast<double>(z) + position.z * CHUNK_SIZE;

            // Use 2D Perlin noise to determine the terrain height
            constexpr double frequency = 0.001;
            constexpr int octaves = 6;
            const double noiseValue = Noise.octave2D_01(worldX * frequency, worldZ * frequency, octaves);

            // Scale the noise to a desired height range
            constexpr int baseHeight = 32;
            constexpr int amplitude = 60;
            const int terrainHeight = baseHeight + static_cast<int>(noiseValue * amplitude);

            // Fill blocks up to the terrain height
            for (unsigned short y = 0; y < CHUNK_SIZE; y++)
            {
                if (const int worldY = y + position.y * CHUNK_SIZE;
                    worldY < terrainHeight)
                {
                    data[x][y][z] = true;
                }
            }
        }
    }
}


bool Chunk::isSolid(const int  x, const int y, const int z) const {
    
    if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) {
        return data[x][y][z]; //inside the chunk
    }
   // return 0;
   // return 0;
    // calculate world pos  and then the neighbr chunk and local block pos
    const int worldX = position.x * CHUNK_SIZE + x;
    const int worldY = position.y * CHUNK_SIZE + y;
    const int worldZ = position.z * CHUNK_SIZE + z;

    const glm::i32vec3 neighborChunkPos(
            // (type)variable -> c static cast -> c++
        std::floor(static_cast<float>(worldX) / CHUNK_SIZE),
        std::floor(static_cast<float>(worldY) / CHUNK_SIZE),
        std::floor(static_cast<float>(worldZ) / CHUNK_SIZE)
    );


    if (const auto it = worldPtr->chunks.find(neighborChunkPos);
        it != worldPtr->chunks.end()) { // if the chunk is not found (does not exist) the variable will be equal to .end()
        const Chunk& neighbor = it->second; // first - position | second - Chunk
        const int localX = worldX - neighbor.position.x * CHUNK_SIZE;
        const int localY = worldY - neighbor.position.y * CHUNK_SIZE;
        const int localZ = worldZ - neighbor.position.z * CHUNK_SIZE;
        return neighbor.isSolid(localX, localY, localZ);

    }  
    return false;

}


void  Chunk::generateMesh() { 
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for  (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z =0; z < CHUNK_SIZE; z++) {
                if (!data[x][y][z]) { 
                    continue; }
                const auto worldPos = glm::i32vec3(
                    position.x * 32 + x,  // CHUNK_SIZE = 32
                    position.y * 32 + y,
                    position.z * 32 + z);


                if (!isSolid(x, y + 1, z)) // UP
                {
                    const std::vector<GLfloat> tempVertices = {
                        (GLfloat)worldPos.x,              (GLfloat)worldPos.y, (GLfloat)worldPos.z,         1, 1,
                        (GLfloat)(worldPos.x + 1), (GLfloat)worldPos.y, (GLfloat)worldPos.z,                0, 0,
                        (GLfloat)(worldPos.x + 1), (GLfloat)worldPos.y, (GLfloat)(worldPos.z - 1),          0, 0,
                        (GLfloat)worldPos.x,              (GLfloat)worldPos.y, (GLfloat)(worldPos.z - 1),   1, 1,
                    };
                    vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
                    addIndices();
                }
                if (!isSolid(x, y - 1, z))
                {
                    const std::vector<GLfloat> tempVertices = {
                        (GLfloat)worldPos.x,              (GLfloat)(worldPos.y - 1), (GLfloat)(worldPos.z - 1), 0, 1,
                        (GLfloat)(worldPos.x + 1), (GLfloat)(worldPos.y - 1), (GLfloat)(worldPos.z - 1), 1, 1,
                        (GLfloat)(worldPos.x + 1), (GLfloat)(worldPos.y - 1), (GLfloat)worldPos.z,              1, 1,
                        (GLfloat)worldPos.x,              (GLfloat)(worldPos.y - 1), (GLfloat)worldPos.z,              0, 1,
                    };
                    vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
                    addIndices();
                }
                if (!isSolid(x + 1, y, z))
                {
                    const std::vector<GLfloat> tempVertices = {
                        (GLfloat)worldPos.x, (GLfloat)(worldPos.y - 1), (GLfloat)worldPos.z,              0, 1,
                        (GLfloat)worldPos.x, (GLfloat)worldPos.y,              (GLfloat)worldPos.z,              1, 1,
                        (GLfloat)worldPos.x, (GLfloat)worldPos.y,              (GLfloat)(worldPos.z - 1), 1, 1,
                        (GLfloat)worldPos.x, (GLfloat)(worldPos.y - 1), (GLfloat)(worldPos.z - 1), 0, 1,
                    };
                    vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
                    addIndices();
                }
                if (!isSolid(x - 1, y, z))
                {
                    const std::vector<GLfloat> tempVertices = {
                        (GLfloat)(worldPos.x + 1), (GLfloat)(worldPos.y - 1), (GLfloat)(worldPos.z - 1), 0, 1,
                        (GLfloat)(worldPos.x + 1), (GLfloat)worldPos.y,              (GLfloat)(worldPos.z - 1), 1, 1,
                        (GLfloat)(worldPos.x + 1), (GLfloat)worldPos.y,              (GLfloat)worldPos.z,              1, 1,
                        (GLfloat)(worldPos.x + 1), (GLfloat)(worldPos.y - 1), (GLfloat)worldPos.z,              0, 1,
                    };
                    vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
                    addIndices();
                }
                if (!isSolid(x, y, z + 1))
                {
                    const std::vector<GLfloat> tempVertices = {
                        (GLfloat)worldPos.x,              (GLfloat)(worldPos.y - 1), (GLfloat)worldPos.z, 0, 1,
                        (GLfloat)(worldPos.x + 1), (GLfloat)(worldPos.y - 1), (GLfloat)worldPos.z, 1, 1,
                        (GLfloat)(worldPos.x + 1), (GLfloat)worldPos.y,              (GLfloat)worldPos.z, 1, 1,
                        (GLfloat)worldPos.x,              (GLfloat)worldPos.y,              (GLfloat)worldPos.z, 0, 1,
                    };
                    vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
                    addIndices();
                }
                if (!isSolid(x, y, z - 1))
                {
                    const std::vector<GLfloat> tempVertices = {
                        (GLfloat)(worldPos.x + 1), (GLfloat)(worldPos.y - 1), (GLfloat)(worldPos.z - 1), 0, 1,
                        (GLfloat)worldPos.x,              (GLfloat)(worldPos.y - 1), (GLfloat)(worldPos.z - 1), 1, 1,
                        (GLfloat)worldPos.x,              (GLfloat)worldPos.y,              (GLfloat)(worldPos.z - 1), 1, 1,
                        (GLfloat)(worldPos.x + 1), (GLfloat)worldPos.y,              (GLfloat)(worldPos.z - 1), 0, 1,
                    };
                    vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
                    addIndices();
                }
            }
        }
    }
}



void Chunk::addIndices(){
        const auto startIndex = static_cast<unsigned int>(vertices.size() / 5); // 5 floats per vertex(3-pos, 2-texture pos)
        std::vector<GLuint> tempIndices = {
            startIndex + 0, startIndex + 1, startIndex + 2,
            startIndex + 0, startIndex + 2, startIndex + 3,
        };
        indices.insert(indices.end(), tempIndices.begin(), tempIndices.end());
    }

void Chunk::allocateMem(const size_t sizeV, const size_t sizeI){ //unsigned long
        if (indices.capacity() < sizeI) {
            indices.reserve(sizeI);
        }
        if (vertices.capacity() < sizeV) {
            vertices.reserve(sizeV);
        }

    }

void Chunk::uploadMesh(){
    vbo.Bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
    ebo.Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);
}

void Chunk::setupBuffers() {
        vao.Bind();

        //pos(3)+texcoord(2)+normal= 5 floats/vertex
        vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);      // position
        vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));  // texcoord

        vao.Unbind();  // Done - VAO now stores VBO attribs + EBO binding
    }

void Chunk::render()
{
        {
            // auto start = std::chrono::high_resolution_clock::now();

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Bind buffers, upload current vertex/index data, then draw
            vbo.Bind();
            vao.Bind();
            ebo.Bind();

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
            //10-50ms
            // auto end = std::chrono::high_resolution_clock::now();
            // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            // std::cout << "Render time: " << duration.count() << " microseconds" << std::endl;
        }
}
