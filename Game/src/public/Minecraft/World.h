#pragma once

#include "Minecraft/World/Chunk.h"


namespace Minecraft {

	class World
	{
	public:
		World(uint64_t seed, const glm::vec3* playerPosition);
		~World();

		void Tick(Timestep ts);

		bool HasChunk(Chunk::Position& pos) const { return m_Chunks.find(pos) != m_Chunks.end(); }
		Ref<Chunk> GetChunk(Chunk::Position& pos) const { return m_Chunks.at(pos); }

		inline uint64_t GetSeed() const { return m_Seed; }

		Chunk::PositionIterator begin() { return Chunk::PositionIterator(m_RenderChunkBase); };
		Chunk::PositionIterator end() { return Chunk::PositionIterator(m_RenderChunkPtr); };

	private:
		void OnCrossChunkBorder();
		void OnCrossBlockBorder();

		/**
		* @return true if done generating all chunks.
		* This is because I only want to generate at most one chunk per tick.
		*/
		bool GenerateNewChunksFromRenderChunks(bool allAtOnce = false);

		void RecalculateRenderChunks();

	private:
		uint64_t m_Seed;
		const glm::vec3* m_PlayerPosition = nullptr;

		Chunk::Position* m_RenderChunkBase;
		Chunk::Position* m_RenderChunkPtr;
		std::unordered_map<Chunk::Position, Ref<Chunk>> m_Chunks;

		// Draw distance in chunks
		uint16_t m_DrawDistance = 10;

		Chunk::Position m_PlayerChunkPosition;
		ChunkBlock::Position m_PlayerBlockPosition;

		bool m_GeneratingNewChunks = false;
	};

}
