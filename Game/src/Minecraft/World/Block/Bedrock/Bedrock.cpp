#include "mcpch.h"
#include "Minecraft/World/Block/Bedrock/Bedrock.h"


namespace Minecraft {

	uint16_t BedrockBlock::GetTextureIndex(Face face) const
	{
		switch (face)
		{
			case Face::Top:			return (uint16_t)Block::TextureIndex::Bedrock;
			case Face::Bottom:		return (uint16_t)Block::TextureIndex::Bedrock;
			case Face::North:		return (uint16_t)Block::TextureIndex::Bedrock;
			case Face::South:		return (uint16_t)Block::TextureIndex::Bedrock;
			case Face::East:		return (uint16_t)Block::TextureIndex::Bedrock;
			case Face::West:		return (uint16_t)Block::TextureIndex::Bedrock;
		}

		ME_ASSERT(false, "Unknown face!");
		return 0;
	}

}
