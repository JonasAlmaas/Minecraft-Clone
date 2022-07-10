#pragma once

using namespace Moon;


namespace Minecraft {

	class World
	{
	public:
		World() = default;
		World(uint64_t seed);

		inline uint64_t GetSeed() const { return m_Seed; }

	private:
		uint64_t m_Seed;

	public:
		Ref<VertexArray> m_VA;

	};

}
