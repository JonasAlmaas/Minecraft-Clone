#pragma once

using namespace Moon;


namespace Minecraft {

	class WorldGen
	{
	public:
		static void Init(int seed);
		static void Shutdown();

		static uint8_t GetHeight(int x, int y);

	};

}

