#include "WorldClass.h"
#include<iostream>
#include"ChunkClass.h"
#include <prelin/PerlinNoise.hpp>
#include <thread>
#include <chrono>

//void World::generateWorld(int size){
//
//	for (int x = 0; x < size; x++) {
//		for (int y = 0; y < 4; y++) {
//			for (int z = 0; z < size; z++) {
//				glm::i32vec3 chunkPos(x, y, z);
//				chunks.insert({ chunkPos, Chunk(chunkPos, noise) });
//			}
//		}
//	}
//}

void World::loadChunks(Renderer& myRenderer) {

	for (auto& [_, chunk] : chunks) {
		myRenderer.renderMesh(chunk.mesh, chunk.position);

	}
}

void World::buildAndUploadMesh(Renderer& myRenderer) {
    myRenderer.clrIndices();
    myRenderer.clrVertices();
	size_t sizeV = 0;
	size_t sizeI = 0;
	for (auto& [_, chunk] : chunks) {
		//							 pos    texture pos     4 cords
		sizeV += chunk.mesh.size() * (3      + 2             + 4);
		sizeI += chunk.mesh.size() * 6; // 6 indicies
	}
	myRenderer.allocateMem(sizeV, sizeI);

    for (auto& [_, chunk] : chunks) {
        myRenderer.renderMesh(chunk.mesh, chunk.position);
    }

    myRenderer.uploadMesh();
}


void World::AppendUploadMesh(Renderer& myRenderer) {
    if (newChunksPos.empty()) {
        return;
    }


    size_t sizeV = 0;
    size_t sizeI = 0;

    for (const glm::i32vec3& pos : newChunksPos) { //0.01ms
        const auto& chunk = chunks.at(pos);
        const size_t meshSize = chunk.mesh.size();
        sizeV += meshSize * (3 + 2 + 4);
        sizeI += meshSize * 6;
    }


    sizeV += myRenderer.getSizeVertices();// adds new size with the
    sizeI += myRenderer.getSizeIndices();

    myRenderer.allocateMem(sizeV, sizeI); //100ms



    for (const glm::i32vec3& pos : newChunksPos) { //500ms
        if (auto it = chunks.find(pos); it != chunks.end()) { //keeping variables local to the if block

            it->second.generateMesh();  // Calls on existing Chunk0
            const Chunk& chunk = chunks.at(pos);
            myRenderer.renderMesh(chunk.mesh, chunk.position);
        }
        else{
            throw std::runtime_error("ERROR: new chunk is empty");
        }


    }
    newChunksPos.clear();
    myRenderer.uploadMesh();


}


void World::generateChunkAround(glm::i32vec3 chunkPos) {
    constexpr short halfRenderDist = RENDER_DISTANCE / 2;
    constexpr int radiusSquared = halfRenderDist * halfRenderDist;
    constexpr int totalChunks = RENDER_DISTANCE * 4 * RENDER_DISTANCE;

    newChunksPos.reserve(newChunksPos.size() + totalChunks);

    glm::i32vec3 currentChunkPos;
    currentChunkPos.y = 0;

    for (short x = -halfRenderDist; x < halfRenderDist; x++) {
        currentChunkPos.x = chunkPos.x + x;
        for (short y = 0; y < 6; y++) {
            currentChunkPos.y = y;
            for (short z = -halfRenderDist; z < halfRenderDist; z++) {
                // check if within radius
                if (const int distSquared = x * x + z * z; distSquared > radiusSquared) { //keeping variables local to the if
                    continue;
                }

                currentChunkPos.z = chunkPos.z + z;

                if (auto [it, inserted] = chunks.try_emplace(currentChunkPos, currentChunkPos, noise, this);
                    inserted) {
                    newChunksPos.push_back(currentChunkPos);
                }
            }
        }
    }
}
