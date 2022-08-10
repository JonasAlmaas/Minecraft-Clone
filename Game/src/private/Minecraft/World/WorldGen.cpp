#include "mcpch.h"
#include "Minecraft/World/WorldGen.h"


namespace Minecraft {

	static Noise* s_WorldNoise;

	void WorldGen::Init(int seed)
	{
		Noise::Specification spec;
		spec.Seed = seed;
		s_WorldNoise = new Noise(spec);
	}

	void WorldGen::Shutdown()
	{
		delete s_WorldNoise;
	}

	uint8_t WorldGen::GetHeight(int x, int y)
	{
		return (uint8_t)((s_WorldNoise->Get((float)x, (float)y) * 0.5 + 0.5) * 64.0f) + 40;
	}

}
