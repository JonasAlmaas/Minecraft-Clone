#include "mcpch.h"
#include "Minecraft/World.h"


namespace Minecraft {

	World::World(const glm::vec3* playerPos)
		: m_PlayerPosition(playerPos)
	{
		// TODO: This has to be recreated every time the draw distance changes
		m_RenderChunkBase = new ChunkPosition[(m_DrawDistance + m_DrawDistance + 1) * (m_DrawDistance + m_DrawDistance + 1)];
		m_RenderChunkPtr = m_RenderChunkBase;

		m_PlayerBlockPosition = {
			(int)m_PlayerPosition->x,
			(int)m_PlayerPosition->y,
			(uint8_t)m_PlayerPosition->z
		};

		m_PlayerChunkPosition = m_PlayerBlockPosition.GetChunkPosition();

		RecalculateRenderChunks();
		m_GeneratingNewChunks = !GenerateNewChunksFromRenderChunks();
	}

	World::~World()
	{
		delete[] m_RenderChunkBase;
	}

	void World::OnUpdate(Timestep ts)
	{
		// Generate only one chunk every frame, this is to prevent massive lag spikes
		if (m_GeneratingNewChunks)
			m_GeneratingNewChunks = !GenerateNewChunksFromRenderChunks();
	}

	void World::Tick(Timestep ts)
	{
		WorldBlockPosition currentPlayerBlockPosition = {
			(int)m_PlayerPosition->x,
			(int)m_PlayerPosition->y,
			(uint8_t)m_PlayerPosition->z
		};

		ChunkPosition currentPlayerChunkPosition = currentPlayerBlockPosition.GetChunkPosition();

		// Move between chunks
		if (currentPlayerChunkPosition != m_PlayerChunkPosition)
		{
			m_PlayerChunkPosition = currentPlayerChunkPosition;
			OnCrossChunkBorder();
		}

		// Move between blocks
		if (currentPlayerBlockPosition != m_PlayerBlockPosition)
		{
			m_PlayerBlockPosition = currentPlayerBlockPosition;
			OnCrossBlockBorder();
		}
	}

	bool World::HasBlock(const WorldBlockPosition& position)
	{
		ChunkPosition chunkPos = position.GetChunkPosition();

		if (!HasChunk(chunkPos))
			return false;

		return GetChunk(chunkPos)->HasBlock(position.GetLocalPosition());
	}

	bool World::HasChunk(const ChunkPosition& position)
	{
		return m_Chunks.find(position) != m_Chunks.end();
	}

	Ref<Chunk> World::GetChunk(const ChunkPosition& position) const
	{
		return m_Chunks.at(position);
	}

	Ref<Block> World::GetBlock(const WorldBlockPosition& position) const
	{
		return GetChunk(position.GetChunkPosition())->GetBlock(position);
	}

	void World::BreakBlock(const WorldBlockPosition& position)
	{
		if (!HasBlock(position))
			return;

		GetChunk(position.GetChunkPosition())->BreakBlock(position);
	}

	void World::OnCrossChunkBorder()
	{
		// Sort chunk render order (The current way this is happening is not great)

		m_GeneratingNewChunks = true;
		RecalculateRenderChunks();
	}

	void World::OnCrossBlockBorder()
	{
		// TODO: Sort transparent blocks within chunk back to front
	}

	bool World::GenerateNewChunksFromRenderChunks()
	{
		for (auto& chunkPosition : *this)
		{
			if (m_Chunks.find(chunkPosition) == m_Chunks.end())
			{
				m_Chunks[chunkPosition] = CreateRef<Chunk>(chunkPosition, this);
				m_Chunks[chunkPosition]->GenerateMeshes();
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
					ChunkPosition chunkPosition = { x, y };
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

}
