#include "mcpch.h"
#include "Minecraft/Application/Application.h"

#include "Minecraft/Layer/GameLayer.h"


namespace Minecraft {

	MinecraftApp::MinecraftApp(const ApplicationSpecification& spec)
		: Application(spec)
	{
		PushLayer(CreateRef<GameLayer>());
	}

}
