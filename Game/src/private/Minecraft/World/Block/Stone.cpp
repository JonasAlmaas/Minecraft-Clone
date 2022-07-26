#include "mcpch.h"
#include "Minecraft/World/Block/Stone.h"


namespace Minecraft {

	uint16_t StoneBlock::GetTextureIndex(Face face) const
	{
		switch (face)
		{
			case Face::Top:			return (uint16_t)Block::TextureIndex::Stone;
			case Face::Bottom:		return (uint16_t)Block::TextureIndex::Stone;
			case Face::North:		return (uint16_t)Block::TextureIndex::Stone;
			case Face::South:		return (uint16_t)Block::TextureIndex::Stone;
			case Face::East:		return (uint16_t)Block::TextureIndex::Stone;
			case Face::West:		return (uint16_t)Block::TextureIndex::Stone;
		}

		ME_ASSERT(false, "Unknown face!");
		return 0;
	}

}
