#include "mcpch.h"
#include "Minecraft/World/Block/Block.h"

#include "Minecraft/World/Block/Bedrock/Bedrock.h"
#include "Minecraft/World/Block/Dirt/Dirt.h"
#include "Minecraft/World/Block/Grass/Grass.h"
#include "Minecraft/World/Block/Stone/Stone.h"


namespace Minecraft {

	Ref<Block> Block::Create(Type blockType)
	{
		switch (blockType)
		{
			case Type::Bedrock:		return CreateRef<BedrockBlock>();
			case Type::Dirt:		return CreateRef<DirtBlock>();
			case Type::Grass:		return CreateRef<GrassBlock>();
			case Type::Stone:		return CreateRef<StoneBlock>();
		}
	
		return CreateRef<GrassBlock>();
	}

}

