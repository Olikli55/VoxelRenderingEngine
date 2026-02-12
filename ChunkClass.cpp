#include"ChunkClass.h"
#include <glm/vec3.hpp>
#include "Direction.h"

    Chunk::Chunk(glm::i32vec3 BasePos, const siv::PerlinNoise& Noise, World* myWorld)
        : position(BasePos), // Store the position
          worldPtr(myWorld)
    {
     //   for (unsigned short x = 0; x < CHUNK_SIZE; x++) {
     //       for (unsigned short z = 0; z < CHUNK_SIZE; z++) {
     //           for (unsigned short y = 0; y < CHUNK_SIZE; y++) {
					//data[x][y][z] = true;
     //           }
     //       }
     //   }
     // 
        for (unsigned short x = 0; x < CHUNK_SIZE; x++) {
            for (unsigned short z = 0; z < CHUNK_SIZE; z++) {
 
                // Calculate world coordinates for the column
                double worldX = static_cast<double>(x) + position.x * CHUNK_SIZE;
                double worldZ = static_cast<double>(z) + position.z * CHUNK_SIZE;

                // Use 2D Perlin noise to determine the terrain height
                double frequency = 0.01;
                int octaves = 4;
                double noiseValue = Noise.octave2D_01(worldX * frequency, worldZ * frequency, octaves);

                // Scale the noise to a desired height range
                int baseHeight = 32;
                int amplitude = 30;
                int terrainHeight = baseHeight + static_cast<int>(noiseValue * amplitude);

                // Fill blocks up to the terrain height
                for (unsigned short y = 0; y < CHUNK_SIZE; y++) {
                    int worldY = y + position.y * CHUNK_SIZE;
                    if (worldY < terrainHeight) {
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
    int worldX = position.x * CHUNK_SIZE + x;
    int worldY = position.y * CHUNK_SIZE + y;
    int worldZ = position.z * CHUNK_SIZE + z;

    glm::i32vec3 neighborChunkPos(
        std::floor((float)worldX / CHUNK_SIZE),
        std::floor((float)worldY / CHUNK_SIZE),
        std::floor((float)worldZ / CHUNK_SIZE)
    );

    auto it = worldPtr->chunks.find(neighborChunkPos);
    if (it != worldPtr->chunks.end()) {
        const Chunk& neighbor = it->second;
        int localX = worldX - neighbor.position.x * CHUNK_SIZE;
        int localY = worldY - neighbor.position.y * CHUNK_SIZE;
        int localZ = worldZ - neighbor.position.z * CHUNK_SIZE;
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


                if (!isSolid(x, y + 1, z)) { mesh.push_back(PackedVector(x,y,z,UP)); }
                if (!isSolid(x, y - 1, z)) { mesh.push_back(PackedVector(x,y,z,DOWN)); }
                if (!isSolid(x + 1, y, z)) { mesh.push_back(PackedVector(x,y,z,RIGHT)); }
                if (!isSolid(x - 1, y, z)) { mesh.push_back(PackedVector(x,y,z,LEFT)); }
                if (!isSolid(x, y, z + 1)) { mesh.push_back(PackedVector(x,y,z,FORWARD)); }
                if (!isSolid(x, y, z - 1)) { mesh.push_back(PackedVector(x,y,z,BACKWARD)); }
            }
        }
    }
}

bool Chunk::getData(glm::i32vec3 pos) {

}

