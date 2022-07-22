#include "mcpch.h"
#include <Moon/Core/Entrypoint.h>

#include "Minecraft/Core/Application/Application.h"


namespace Moon {

	Application* Application::Create(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.CommandLineArgs = args;
		spec.Name = "Minecraft";
		spec.WorkingDirectory = "../Game";
		spec.WindowWidth = 1920;
		spec.WindowHeight = 1080;

		return new Minecraft::MinecraftApp(spec);
	}

}
