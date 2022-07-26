#pragma once

using namespace Moon;


namespace Minecraft {

	class Block
	{
	public:
		enum class Type
		{
			Air = 0,
			Bedrock,
			Cobblestone,
			CraftingTable,
			Dirt,
			Glass,
			GlassPaneTop,
			Grass,
			Gravel,
			OakLog,
			OakPlank,
			Sand,
			Stone,
			Tourch,
		};

		enum class TextureIndex
		{
			GrassTop = 0,
			GrassSide,
			Dirt,
			Cobblestone,
			Gravel,
			Sand,
			Bedrock,
			OakLogSide,
			OakLogTop,
			OakPlank,
			CraftingTableSide1,
			CraftingTableSide2,
			CraftingTableTop,
			RedstoneLampOff,
			RedstoneLampOn,
			Tourch,
			Stone,
			CopperOre,
			DiamondOre,
			EmeraldOre,
			IronOre,
			GoldOre,
			LapisOre,
			RedstoneOre,
			Glass,
			GlassSide,
		};

		enum class Face
		{
			Top,
			Bottom,
			North,
			South,
			East,
			West,
		};

		enum class Renderpass
		{
			Solid = 0,
			Transparant,
			Water,
		};

	public:
		virtual ~Block() = default;

		static Ref<Block> Create(Type blockType);

		virtual uint16_t GetTextureIndex(Face face) const = 0;
		virtual Renderpass GetRenderpass() const = 0;

	};

}
