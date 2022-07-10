#include "mcpch.h"
#include "Minecraft/Core/Application/Application.h"

#include "Minecraft/Core/Layer/GameLayer.h"


namespace Minecraft {

	MinecraftApp::MinecraftApp(const ApplicationSpecification& spec)
		: Application(spec)
	{
		PushLayer(CreateRef<GameLayer>());
	}

}
