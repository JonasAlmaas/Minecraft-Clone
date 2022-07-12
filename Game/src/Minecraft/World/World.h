#pragma once

#include "Minecraft/World/Chunk/Chunk.h"


namespace Minecraft {

	class World
	{
	public:
		World();
		World(uint64_t seed);

		inline uint64_t GetSeed() const { return m_Seed; }

	private:
		void GenerateWorld();

	public:
		std::vector<Ref<Chunk>> m_Chunks;

	private:
		uint64_t m_Seed;

	};

}
