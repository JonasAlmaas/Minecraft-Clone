#pragma once

#include "Minecraft/World/Chunk.h"


namespace Minecraft {

	class World
	{
	public:
		World(const glm::vec3* playerPosition);
		~World();

		// Called every frame
		void OnUpdate(Timestep ts);

		// Called 20 times a secon
		void Tick(Timestep ts);

		bool HasBlock(const WorldBlockPosition& position);
		bool HasChunk(const ChunkPosition& position);

		Ref<Chunk> GetChunk(const ChunkPosition& position) const;
		Ref<Block> GetBlock(const WorldBlockPosition& position) const;

		void BreakBlock(const WorldBlockPosition& position);

		ChunkPosition::Iterator begin() { return ChunkPosition::Iterator(m_RenderChunkBase); };
		ChunkPosition::Iterator end() { return ChunkPosition::Iterator(m_RenderChunkPtr); };
		
	private:
		void OnCrossChunkBorder();
		void OnCrossBlockBorder();

		/**
		* @return true if done generating all chunks.
		* This is because I only want to generate at most one chunk per tick.
		*/
		bool GenerateNewChunksFromRenderChunks();

		void RecalculateRenderChunks();

	private:
		const glm::vec3* m_PlayerPosition;

		ChunkPosition* m_RenderChunkBase;
		ChunkPosition* m_RenderChunkPtr;

		std::unordered_map<ChunkPosition, Ref<Chunk>> m_Chunks;

		// Draw distance in chunks
		uint16_t m_DrawDistance = 12;

		ChunkPosition m_PlayerChunkPosition;
		WorldBlockPosition m_PlayerBlockPosition;

		bool m_GeneratingNewChunks = false;

	};

}
