#pragma once

using namespace Moon;

#include "Minecraft/World.h"


namespace Minecraft {

	class GameRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void ReloadShaders();

		static void RenderWorld(const Ref<World>& world, const glm::mat4& viewProjectionMatrix);
		static void RenderHUD(const Ref<RenderCamera>& renderCamera);

	};

}
