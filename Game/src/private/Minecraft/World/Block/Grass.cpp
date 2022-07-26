#include "mcpch.h"
#include "Minecraft/World/Block/Grass.h"


namespace Minecraft {

	uint16_t GrassBlock::GetTextureIndex(Face face) const
	{
		switch (face)
		{
			case Face::Top:			return (uint16_t)Block::TextureIndex::GrassTop;
			case Face::Bottom:		return (uint16_t)Block::TextureIndex::Dirt;
			case Face::North:		return (uint16_t)Block::TextureIndex::GrassSide;
			case Face::South:		return (uint16_t)Block::TextureIndex::GrassSide;
			case Face::East:		return (uint16_t)Block::TextureIndex::GrassSide;
			case Face::West:		return (uint16_t)Block::TextureIndex::GrassSide;
		}

		ME_ASSERT(false, "Unknown face!");
		return 0;
	}

}
