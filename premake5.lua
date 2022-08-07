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
	include "Engine/Moon-Engine/vendor/Box2D"
	include "Engine/Moon-Engine/vendor/Glad"
	include "Engine/Moon-Engine/vendor/GLFW"
	include "Engine/Moon-Engine/vendor/ImGui"
	include "Engine/Moon-Engine/vendor/msdf-atlas-gen"
	include "Engine/Moon-Engine/vendor/yaml"
group ""

include "Engine/Moon-Engine"
include "Game"
