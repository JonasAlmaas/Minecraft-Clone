#include "mcpch.h"
#include "Minecraft/World/World.h"


namespace Minecraft {

	World::World(uint64_t seed, const glm::vec3* playerPos)
		: m_Seed(seed), m_PlayerPosition(playerPos)
	{
		// TODO: This has to be recreated every time the draw distance changes
		m_RenderChunkBase = new Chunk::Position[(m_DrawDistance + m_DrawDistance + 1) * (m_DrawDistance + m_DrawDistance + 1)];
		m_RenderChunkPtr = m_RenderChunkBase;

		GenerateWorld();
	}

	World::~World()
	{
		delete[] m_RenderChunkBase;
	}

	void World::Tick(Timestep ts)
	{
		Chunk::Position currentPlayerChunkPosition = {
			(int)glm::floor(m_PlayerPosition->x / 16.0f),
			(int)glm::floor(m_PlayerPosition->y / 16.0f)
		};

		ChunkBlock::Position currentPlayerBlockPosition = {
			(int)glm::floor(m_PlayerPosition->x) % 16,
			(int)glm::floor(m_PlayerPosition->y) % 16,
			(int)glm::floor(m_PlayerPosition->z)
		};

		// Move between chunks
		if (currentPlayerChunkPosition != m_PlayerChunkPosition)
		{
			m_PlayerChunkPosition = currentPlayerChunkPosition;
			//m_GeneratingNewChunks = true;

			//OnCrossChunkBorder();
		}

		// Move between blocks
		if (currentPlayerBlockPosition != m_PlayerBlockPosition)
		{
			m_PlayerBlockPosition = currentPlayerBlockPosition;
			OnCrossBlockBorder();
		}

		// Generate only one chunk every second, this is to prevent massive lag spikes
		if (m_GeneratingNewChunks)
			m_GeneratingNewChunks = !GenerateNewChunksFromRenderChunks();
	}

	void World::OnCrossChunkBorder()
	{
		RecalculateRenderChunks();
	}

	void World::OnCrossBlockBorder()
	{
		// TODO: Sort transparent blocks within chunk back to front
	}

	bool World::GenerateNewChunksFromRenderChunks(bool allAtOnce)
	{
		for (auto& chunkPosition : *this)
		{
			if (m_Chunks.find(chunkPosition) == m_Chunks.end())
			{
				m_Chunks[chunkPosition] = CreateRef<Chunk>(chunkPosition);

				if (!allAtOnce)
					return false;
			}
		}

		return true;
	}

	void World::RecalculateRenderChunks()
	{
		/**
		 * Added 0.5 to the radius, see link bellow to reasoning.
		 * https://www.redblobgames.com/grids/circle-drawing/#aesthetics
		 */
		float radius = ((float)m_DrawDistance) + 0.5f;

		// Reset render chunk pointer
		m_RenderChunkPtr = m_RenderChunkBase;

		for (uint16_t i = 0; i < m_DrawDistance; i++)
		{
			for (int x = m_PlayerChunkPosition.x - i; x <= m_PlayerChunkPosition.x + i; x++)
			{
				for (int y = m_PlayerChunkPosition.y - i; y <= m_PlayerChunkPosition.y + i; y++)
				{
					Chunk::Position chunkPosition = { x, y };
					if (chunkPosition.IsWithinRadius(m_PlayerChunkPosition, radius))
					{
						m_RenderChunkPtr->x = x;
						m_RenderChunkPtr->y = y;
						m_RenderChunkPtr++;
					}

					if (y != m_PlayerChunkPosition.y + i && x != m_PlayerChunkPosition.x - i && x != m_PlayerChunkPosition.x + i)
						y = m_PlayerChunkPosition.y + i - 1;
				}
			}
		}
	}

	void World::GenerateWorld()
	{
		//RecalculateRenderChunks();

		for (int x = 0; x <= 16; x++)
		{
			Chunk::Position chunkPosition = { x, 0 };
			m_RenderChunkPtr->x = x;
			m_RenderChunkPtr->y = 0;
			m_RenderChunkPtr++;
		}

		GenerateNewChunksFromRenderChunks(true);
	}

}
