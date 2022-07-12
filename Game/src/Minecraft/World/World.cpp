#include "mcpch.h"
#include "Minecraft/World/World.h"


namespace Minecraft {

	World::World()
		: m_Seed(Random::UInt())
	{
		GenerateWorld();
	}

	World::World(uint64_t seed)
		: m_Seed(seed)
	{
		GenerateWorld();
	}

	void World::GenerateWorld()
	{
		for (uint32_t x = 0; x < 6; x++)
		{
			for (uint32_t y = 0; y < 6; y++)
			{
				m_Chunks.emplace_back(CreateRef<Chunk>(Int2(x, y)));
			}
		}
	}

}
