include "./Engine/vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Minecraft"
    architecture "x86_64"
    startproject "Game"

    configurations
	{
		"Debug",
		"Release",
		"Dist",
	}

    solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Engine/Engine/vendor/Box2D"
	include "Engine/Engine/vendor/Glad"
	include "Engine/Engine/vendor/GLFW"
	include "Engine/Engine/vendor/ImGui"
	include "Engine/Engine/vendor/yaml"
group ""

include "Engine/Engine"
include "Game"
