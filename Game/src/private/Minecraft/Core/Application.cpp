#include "mcpch.h"
#include "Minecraft/Core/Application.h"

#include "Minecraft/GameLayer.h"


namespace Minecraft {

	MinecraftApp::MinecraftApp(const ApplicationSpecification& spec)
		: Application(spec)
	{
		PushLayer(new GameLayer());
	}

}
