#include "mcpch.h"
#include <Moon/Core/Entrypoint.h>

#include "Minecraft/Core/Application/Application.h"


namespace Moon {

	Application* Application::Create(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Minecraft";
		spec.WorkingDirectory = "../Game";
		spec.CommandLineArgs = args;

		return new Minecraft::MinecraftApp(spec);
	}

}
