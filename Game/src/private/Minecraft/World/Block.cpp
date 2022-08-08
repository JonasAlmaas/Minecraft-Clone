#include "mcpch.h"
#include "Minecraft/World/Block.h"


namespace Minecraft {

	Ref<Block> Block::Create(Type blockType)
	{
		switch (blockType)
		{
			case Type::Bedrock:		return CreateRef<BedrockBlock>();
			case Type::Dirt:		return CreateRef<DirtBlock>();
			case Type::Grass:		return CreateRef<GrassBlock>();
			case Type::Stone:		return CreateRef<StoneBlock>();
			case Type::Water:		return CreateRef<WaterBlock>();
		}
	
		return CreateRef<GrassBlock>();
	}

	// -- Blocks --

	uint16_t BedrockBlock::GetTextureIndex(Face face) const
	{
		return (uint16_t)Block::TextureIndex::Bedrock;
	}

	uint16_t DirtBlock::GetTextureIndex(Face face) const
	{
		return (uint16_t)Block::TextureIndex::Dirt;
	}

	uint16_t GrassBlock::GetTextureIndex(Face face) const
	{
		switch (face)
		{
		case Face::Top:			return (uint16_t)Block::TextureIndex::GrassTop;
		case Face::Bottom:		return (uint16_t)Block::TextureIndex::Dirt;
		case Face::North:
		case Face::South:
		case Face::East:
		case Face::West:		return (uint16_t)Block::TextureIndex::GrassSide;
		}

		ME_ASSERT(false, "Unknown face!");
		return 0;
	}

	uint16_t StoneBlock::GetTextureIndex(Face face) const
	{
		return (uint16_t)Block::TextureIndex::Stone;
	}

	uint16_t WaterBlock::GetTextureIndex(Face face) const
	{
		return (uint16_t)Block::TextureIndex::Water;
	}

}
