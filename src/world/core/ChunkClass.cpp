#include"ChunkClass.h"
#include <glm/vec3.hpp>
#include "Direction.h"

    Chunk::Chunk(glm::i32vec3 BasePos, const siv::PerlinNoise& Noise, World* myWorld)
    : VAO(0), VBO(0), EBO(0), position(BasePos), // Store the position
      worldPtr(myWorld)

{
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
                const int worldY = y + position.y * CHUNK_SIZE;
                if (worldY < terrainHeight)
                {
                    data[x][y][z] = true;
                }
            }
        }
    }
}


bool Chunk::isSolid(int x, int y, int z) const {
    
    if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) {
        return data[x][y][z]; //inside the chunk
    }
   // return 0;
   // return 0;
    // calculate world cordinates and then the neighbr chunk and local block c  ordinates
    const int worldX = position.x * CHUNK_SIZE + x;
    const int worldY = position.y * CHUNK_SIZE + y;
    const int worldZ = position.z * CHUNK_SIZE + z;

    glm::i32vec3 neighborChunkPos(
        std::floor((float)worldX / CHUNK_SIZE),
        std::floor((float)worldY / CHUNK_SIZE),
        std::floor((float)worldZ / CHUNK_SIZE)
    );

    auto it = worldPtr->chunks.find(neighborChunkPos);
    if (it != worldPtr->chunks.end()) {
        const Chunk& neighbor = it->second;
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
        const auto startIndex = static_cast<unsigned int>(vertices.size() / 5); // 5 floats per vertex
        std::vector<GLuint> tempIndices = {
            startIndex + 0, startIndex + 1, startIndex + 2,
            startIndex + 0, startIndex + 2, startIndex + 3,
        };
        indices.insert(indices.end(), tempIndices.begin(), tempIndices.end());
    }

void Chunk::allocateMem(size_t sizeV, size_t sizeI) { //unsigned long a

        //   only reserve if it needs more capacity than it has
        if (indices.capacity() < sizeI) {
            indices.reserve(sizeI);
        }

        if (vertices.capacity() < sizeV) {
            vertices.reserve(sizeV);
        }

    }