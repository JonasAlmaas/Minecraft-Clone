#pragma once

using namespace Moon;

#include "Minecraft/World/World.h"


namespace Minecraft {

	class Renderer
	{
	public:
		static void Init();

		static void RenderWorld(const Ref<World>& world, const glm::mat4& viewProjectionMatrix);

	};

}
