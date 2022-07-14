#include "mcpch.h"
#include "Minecraft/World/World.h"


namespace Minecraft {

	World::World(uint64_t seed, const glm::vec3* playerPos)
		: m_Seed(seed), m_PlayerPosition(playerPos)
	{
		GenerateWorld();
	}

	void World::Tick(Timestep ts)
	{
		static int chunkDrawDistance = 4;

		Int2 currentChunkPosition = { (int)glm::floor(m_PlayerPosition->x / 16.0f), (int)glm::floor(m_PlayerPosition->y / 16.0f) };

		// Move between chunks
		if (currentChunkPosition != m_LastChunkPosition)
		{
			m_LastChunkPosition = currentChunkPosition;
			m_GeneratingNewChunks = true;
		}

		if (m_GeneratingNewChunks)
			m_GeneratingNewChunks = !GenerateNewChunks();

		//Int2 currentBlockPosition = { glm::floor<int>(m_PlayerPosition->x), glm::floor<int>(m_PlayerPosition->y / 16.0f) };

		// Move between blocks
		//ME_CORE_INFO("OnMoveToNewBlock, {}, {}, {}", m_LastBlockPosition.X, m_LastBlockPosition.Y, m_LastBlockPosition.Z);
	}

	bool World::GenerateNewChunks()
	{
		static int chunkDrawDistance = 4;

		for (int32_t x = m_LastChunkPosition.X - chunkDrawDistance; x < m_LastChunkPosition.X + chunkDrawDistance; x++)
		{
			for (int32_t y = m_LastChunkPosition.Y - chunkDrawDistance; y < m_LastChunkPosition.Y + chunkDrawDistance; y++)
			{
				if (m_Chunks.find({ x, y }) == m_Chunks.end())
				{
					m_Chunks[{ x, y }] = CreateRef<Chunk>(Chunk::Position(x, y));
					m_RenderChunks.push_back(m_Chunks[{ x, y }]);
					return false;
				}
			}
		}

		return true;
	}

	void World::GenerateWorld()
	{
		for (int32_t x = -3; x < 3; x++)
		{
			for (int32_t y = -3; y < 3; y++)
			{
				m_Chunks[{ x, y }] = CreateRef<Chunk>(Chunk::Position(x, y));
				m_RenderChunks.push_back(m_Chunks[{ x, y }]);
			}
		}
	}

}
