#pragma once

using namespace Moon;


namespace Minecraft {

	class World;
	struct ChunkPosition;

	using LocalBlockPosition = Int3;

	struct WorldBlockPosition
	{
		int x, y;
		uint8_t z;

		WorldBlockPosition()
			: x(0), y(0), z(0)
		{
		}

		WorldBlockPosition(int x, int y, uint8_t z)
			: x(x), y(y), z(z)
		{
		}

		LocalBlockPosition GetLocalPosition() const;
		ChunkPosition GetChunkPosition() const;

		bool operator == (const WorldBlockPosition& other) const { return x == other.x && y == other.y && z == other.z; }
		bool operator != (const WorldBlockPosition& other) const { return !(*this == other); }

		WorldBlockPosition operator+(const WorldBlockPosition& other) const { return WorldBlockPosition(x + other.x, y + other.y, z + other.z); }
		WorldBlockPosition operator-(const WorldBlockPosition& other) const { return WorldBlockPosition(x - other.x, y - other.y, z - other.z); }
	};

	struct TerrainVertex
	{
		uint32_t Pack1;
		uint32_t Color;
	};

	// TODO: Think about how other things are going to be done
	//struct Vertex
	//{
	//	glm::vec3 Position;
	//	glm::vec3 Normal;
	//	float TextureIndex;
	//	// Make type color
	//	uint32_t Color;
	//};

	//struct WaterVertex
	//{
	//	glm::vec3 Position;
	//	glm::vec3 Normal;
	//	uint32_t Color;
	//};

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
			Water,
		};

	public:
		virtual ~Block() = default;

		static Ref<Block> Create(Type blockType);

		virtual TextureIndex GetTextureIndex(Face face) const = 0;
		virtual Renderpass GetRenderpass() const = 0;

		TerrainVertex* GenerateTerrainMesh(Renderpass renderpass, TerrainVertex* vbPtr, World* world, const WorldBlockPosition& position);

	};

	// -- Blocks --

	class BedrockBlock : public Block
	{
	public:
		BedrockBlock() = default;

		virtual TextureIndex GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Solid; }

	};

	class CobblestoneBlock : public Block
	{
	public:
		CobblestoneBlock() = default;

		virtual TextureIndex GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Solid; }

	};

	class GlassBlock : public Block
	{
	public:
		GlassBlock() = default;

		virtual TextureIndex GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Transparant; }

	};

	class DirtBlock : public Block
	{
	public:
		DirtBlock() = default;

		virtual TextureIndex GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Solid; }

	};

	class GrassBlock : public Block
	{
	public:
		GrassBlock() = default;

		virtual TextureIndex GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Solid; }

	};

	class StoneBlock : public Block
	{
	public:
		StoneBlock() = default;

		virtual TextureIndex GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Solid; }

	};

	class WaterBlock : public Block
	{
	public:
		WaterBlock() = default;

		virtual TextureIndex GetTextureIndex(Face face) const override;
		virtual Renderpass GetRenderpass() const override { return Renderpass::Transparant; }
		//virtual Renderpass GetRenderpass() const override { return Renderpass::Water; }

	};

}

namespace std {

	// X and Y can just be in the range of 0 - 15 (4 bit uint)
	// Z is in the range 0 - 255 (8 bit uint)
	// This means that I can hash the block position buy putting them all in a uint16
	template<>
	struct hash<Minecraft::LocalBlockPosition>
	{
		size_t operator()(const Minecraft::LocalBlockPosition& key) const
		{
			return hash<uint16_t>()(key.x + (key.y << 4) + (key.z << 8));
		}
	};

}
