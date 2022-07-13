#include "mcpch.h"
#include "Minecraft/World/Block/Dirt/Dirt.h"


namespace Minecraft {

	uint16_t DirtBlock::GetTextureIndex(Face face) const
	{
		switch (face)
		{
			case Face::Top:			return (uint16_t)Block::TextureIndex::Dirt;
			case Face::Bottom:		return (uint16_t)Block::TextureIndex::Dirt;
			case Face::North:		return (uint16_t)Block::TextureIndex::Dirt;
			case Face::South:		return (uint16_t)Block::TextureIndex::Dirt;
			case Face::East:		return (uint16_t)Block::TextureIndex::Dirt;
			case Face::West:		return (uint16_t)Block::TextureIndex::Dirt;
		}

		ME_ASSERT(false, "Unknown face!");
		return 0;
	}

}
