#pragma once

#include "Minecraft/World/Chunk/Chunk.h"


namespace Minecraft {

	class World
	{
	public:
		World(uint64_t seed, const glm::vec3* playerPosition);

		void Tick(Timestep ts);

		inline uint64_t GetSeed() const { return m_Seed; }

	private:
		void GenerateWorld();

		/**
		* @return true if done generating all chunks.
		* This is because I only want to generate at most one chunk per tick.
		*/
		bool GenerateNewChunks();

	public:
		std::vector<Ref<Chunk>> m_RenderChunks;

	private:
		uint64_t m_Seed;
		const glm::vec3* m_PlayerPosition = nullptr;

		std::unordered_map<Chunk::Position, Ref<Chunk>> m_Chunks;

		Int2 m_LastChunkPosition;
		//Int3 m_LastBlockPosition;

		bool m_GeneratingNewChunks = false;
	};

}
