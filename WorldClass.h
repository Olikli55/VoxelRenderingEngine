#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H
#include <glm/vec3.hpp>
#include <vector>
#include <prelin/PerlinNoise.hpp>
#include <unordered_map>
#include "RendererClass.h"
#include "PackedVector.h"
#include "ChunkClass.h"

class Chunk;

inline bool operator==(const glm::i32vec3& a, const glm::i32vec3& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

namespace std {
    template<>
    struct hash<glm::i32vec3> {
        size_t operator()(const glm::i32vec3& pos) const {
            // Spatial hash using prime numbers
            return ((size_t)pos.x * 73856093) ^ 
                   ((size_t)pos.y * 19349663) ^ 
                   ((size_t)pos.z * 83492791);
        }
    };
}

class World {
    const short RENDER_DISTANCE = 21;
public:
    const siv::PerlinNoise noise{ 232323 };
    std::unordered_map<glm::i32vec3, Chunk> chunks;  // Key = position, Value = chunk data
    void loadChunks(Renderer& myRenderer);
    void buildAndUploadMesh(Renderer& myRenderer);
    void generateChunkAround(glm::i32vec3 chunkPos);
    void addandUploadMesh(Renderer& myRenderer);

private:
    std::vector<glm::i32vec3> newChunksPos;

};

#endif