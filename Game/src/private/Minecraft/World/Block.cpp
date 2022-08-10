#include "mcpch.h"
#include "Minecraft/World/Block.h"

#include "Minecraft/World.h"


namespace Minecraft {

	static uint32_t CreateTerrainPack1(const LocalBlockPosition& localBlockPos, const glm::bvec3& vertexOffset, const glm::bvec2 & uv, Block::TextureIndex texIndex, Block::Face face)
	{
		uint32_t packedPosition = (uint32_t)((uint8_t)localBlockPos.x + (((uint8_t)localBlockPos.y) << 4) + (((uint8_t)localBlockPos.z) << 8)); // 16 bit
		uint32_t packedOffset = (uint32_t)(vertexOffset.x + (vertexOffset.y << 1) + (vertexOffset.z << 2)); // 3 bit
		uint32_t packedUV = (uint32_t)(uv.x + (uv.y <<1)); // 2 bit;
		return packedPosition + (packedOffset << 16) + (packedUV << 19) + (((uint8_t)texIndex) << 21) + (((uint8_t)face) << 29);
	}

	static uint32_t CreateColorPack(const glm::uvec4& color)
	{
		return color.r + (color.g << 8) + (color.b << 16) + (color.a << 24);
	}

	Ref<Block> Block::Create(Type blockType)
	{
		switch (blockType)
		{
			case Type::Bedrock:			return CreateRef<BedrockBlock>();
			case Type::Cobblestone:		return CreateRef<CobblestoneBlock>();
			case Type::Dirt:			return CreateRef<DirtBlock>();
			case Type::Glass:			return CreateRef<GlassBlock>();
			case Type::Grass:			return CreateRef<GrassBlock>();
			case Type::Stone:			return CreateRef<StoneBlock>();
			case Type::Water:			return CreateRef<WaterBlock>();
		}
	
		return CreateRef<GrassBlock>();
	}

	TerrainVertex* Block::GenerateTerrainMesh(Renderpass renderpass, TerrainVertex* vbPtr, World* world, const WorldBlockPosition& position)
	{
		bool hasTopFace = position.z == 255;
		{
			WorldBlockPosition neighborBlockPos = position + WorldBlockPosition(0, 0, 1);
			if (world->HasBlock(neighborBlockPos))
			{
				Renderpass neighborRenderPass = world->GetBlock(neighborBlockPos)->GetRenderpass();
				hasTopFace |= neighborRenderPass != renderpass && neighborRenderPass != Renderpass::Solid;
			}
			else
				hasTopFace = true;
		}

		bool hasBottomFace = position.z == 0;
		{
			WorldBlockPosition neighborBlockPos = position - WorldBlockPosition(0, 0, 1);
			if (world->HasBlock(neighborBlockPos))
			{
				Renderpass neighborRenderPass = world->GetBlock(neighborBlockPos)->GetRenderpass();
				hasBottomFace |= neighborRenderPass != renderpass && neighborRenderPass != Renderpass::Solid;
			}
			else
				hasBottomFace = true;
		}

		bool hasNorthFace = false;
		{
			WorldBlockPosition neighborBlockPos = position + WorldBlockPosition(0, 1, 0);
			if (world->HasBlock(neighborBlockPos))
			{
				Renderpass neighborRenderPass = world->GetBlock(neighborBlockPos)->GetRenderpass();
				hasNorthFace |= neighborRenderPass != renderpass && neighborRenderPass != Renderpass::Solid;
			}
			else
				hasNorthFace = true;
		}

		bool hasSouthFace = false;
		{
			WorldBlockPosition neighborBlockPos = position - WorldBlockPosition(0, 1, 0);
			if (world->HasBlock(neighborBlockPos))
			{
				Renderpass neighborRenderPass = world->GetBlock(neighborBlockPos)->GetRenderpass();
				hasSouthFace |= neighborRenderPass != renderpass && neighborRenderPass != Renderpass::Solid;
			}
			else
				hasSouthFace = true;
		}

		bool hasEastFace = false;
		{
			WorldBlockPosition neighborBlockPos = position + WorldBlockPosition(1, 0, 0);
			if (world->HasBlock(neighborBlockPos))
			{
				Renderpass neighborRenderPass = world->GetBlock(neighborBlockPos)->GetRenderpass();
				hasEastFace |= neighborRenderPass != renderpass && neighborRenderPass != Renderpass::Solid;
			}
			else
				hasEastFace = true;
		}

		bool hasWestFace = false;
		{
			WorldBlockPosition neighborBlockPos = position - WorldBlockPosition(1, 0, 0);
			if (world->HasBlock(neighborBlockPos))
			{
				Renderpass neighborRenderPass = world->GetBlock(neighborBlockPos)->GetRenderpass();
				hasWestFace |= neighborRenderPass != renderpass && neighborRenderPass != Renderpass::Solid;
			}
			else
				hasWestFace = true;
		}

		LocalBlockPosition localPos = position.GetLocalPosition();
		uint32_t color = CreateColorPack({ 0, 0, 0, 0 });

		// -- Top --
		if (hasTopFace)
		{
			TextureIndex texIndex = GetTextureIndex(Face::Top);

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 0, 0, 1 }, { 0, 0 }, texIndex, Face::Top);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 1, 0, 1 }, { 1, 0 }, texIndex, Face::Top);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 1, 1, 1 }, { 1, 1 }, texIndex, Face::Top);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 0, 1, 1 }, { 0, 1 }, texIndex, Face::Top);
			vbPtr->Color = color;
			vbPtr++;
		}

		// -- Bottom --
		if (hasBottomFace)
		{
			TextureIndex texIndex = GetTextureIndex(Face::Bottom);

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 0, 1, 0 }, { 1, 1 }, texIndex, Face::Bottom);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 1, 1, 0 }, { 0, 1 }, texIndex, Face::Bottom);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 1, 0, 0 }, { 0, 0 }, texIndex, Face::Bottom);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 0, 0, 0 }, { 1, 0 }, texIndex, Face::Bottom);
			vbPtr->Color = color;
			vbPtr++;
		}

		// -- North --
		if (hasNorthFace)
		{
			TextureIndex texIndex = GetTextureIndex(Face::North);

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 1, 1, 0 }, { 0, 0 }, texIndex, Face::North);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 0, 1, 0 }, { 1, 0 }, texIndex, Face::North);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 0, 1, 1 }, { 1, 1 }, texIndex, Face::North);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 1, 1, 1 }, { 0, 1 }, texIndex, Face::North);
			vbPtr->Color = color;
			vbPtr++;
		}

		// -- South --
		if (hasSouthFace)
		{
			TextureIndex texIndex = GetTextureIndex(Face::South);

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 0, 0, 0 }, { 0, 0 }, texIndex, Face::South);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 1, 0, 0 }, { 1, 0 }, texIndex, Face::South);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 1, 0, 1 }, { 1, 1 }, texIndex, Face::South);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 0, 0, 1 }, { 0, 1 }, texIndex, Face::South);
			vbPtr->Color = color;
			vbPtr++;
		}

		// -- East --
		if (hasEastFace)
		{
			TextureIndex texIndex = GetTextureIndex(Face::East);

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 1, 0, 0 }, { 0, 0 }, texIndex, Face::East);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 1, 1, 0 }, { 1, 0 }, texIndex, Face::East);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 1, 1, 1 }, { 1, 1 }, texIndex, Face::East);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 1, 0, 1 }, { 0, 1 }, texIndex, Face::East);
			vbPtr->Color = color;
			vbPtr++;
		}

		// -- West --
		if (hasWestFace)
		{
			TextureIndex texIndex = GetTextureIndex(Face::West);

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 0, 1, 0 }, { 0, 0 }, texIndex, Face::West);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 0, 0, 0 }, { 1, 0 }, texIndex, Face::West);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 0, 0, 1 }, { 1, 1 }, texIndex, Face::West);
			vbPtr->Color = color;
			vbPtr++;

			vbPtr->Pack1 = CreateTerrainPack1(localPos, { 0, 1, 1 }, { 0, 1 }, texIndex, Face::West);
			vbPtr->Color = color;
			vbPtr++;
		}

		return vbPtr;
	}

	// -- Blocks --

	Block::TextureIndex BedrockBlock::GetTextureIndex(Face face) const
	{
		return TextureIndex::Bedrock;
	}

	Block::TextureIndex CobblestoneBlock::GetTextureIndex(Face face) const
	{
		return TextureIndex::Cobblestone;
	}

	Block::TextureIndex DirtBlock::GetTextureIndex(Face face) const
	{
		return TextureIndex::Dirt;
	}

	Block::TextureIndex GlassBlock::GetTextureIndex(Face face) const
	{
		return TextureIndex::Glass;
	}

	Block::TextureIndex GrassBlock::GetTextureIndex(Face face) const
	{
		switch (face)
		{
			case Face::Top:			return TextureIndex::GrassTop;
			case Face::Bottom:		return TextureIndex::Dirt;
			case Face::North:
			case Face::South:
			case Face::East:
			case Face::West:		return TextureIndex::GrassSide;
		}

		ME_ASSERT(false, "Unknown face!");
		return TextureIndex::Dirt;
	}

	Block::TextureIndex StoneBlock::GetTextureIndex(Face face) const
	{
		return TextureIndex::Stone;
	}

	Block::TextureIndex WaterBlock::GetTextureIndex(Face face) const
	{
		return TextureIndex::Water;
	}

	// -- World Block Position --
	LocalBlockPosition WorldBlockPosition::GetLocalPosition() const
	{
		LocalBlockPosition result = {
			(glm::abs(x) % 16),
			(glm::abs(y) % 16),
			z
		};

		if (x < 0 && result.x != 0)
			result.x = 16 - result.x;

		if (y < 0 && result.y != 0)
			result.y = 16 - result.y;

		return result;
	}

	ChunkPosition WorldBlockPosition::GetChunkPosition() const
	{
		return {
			x >= 0 ? x / 16 : -((glm::abs(x) + 15) / 16),
			y >= 0 ? y / 16 : -((glm::abs(y) + 15) / 16)
		};
	}

}
