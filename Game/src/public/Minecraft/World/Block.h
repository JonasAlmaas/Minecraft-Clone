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
			Water,
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
			Water,
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
		};

	public:
		virtual ~Block() = default;

		static Ref<Block> Create(Type blockType);

		virtual uint16_t GetTextureIndex(Face face) const = 0;
		virtual Renderpass GetRenderpass() const = 0;

	};

	// -- Blocks --

	class BedrockBlock : public Block
	{
	public:
		BedrockBlock() = default;

		virtual uint16_t GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Solid; }

	};

	class DirtBlock : public Block
	{
	public:
		DirtBlock() = default;

		virtual uint16_t GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Solid; }

	};

	class GrassBlock : public Block
	{
	public:
		GrassBlock() = default;

		virtual uint16_t GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Solid; }

	};

	class StoneBlock : public Block
	{
	public:
		StoneBlock() = default;

		virtual uint16_t GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Solid; }

	};

	class WaterBlock : public Block
	{
	public:
		WaterBlock() = default;

		virtual uint16_t GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Transparant; }

	};

}
