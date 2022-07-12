#pragma once


namespace Minecraft::Block {

	enum class Type
	{
		None = 0,
		Air,
		Dirt,
		Grass,
		Stone,
	};

	enum class Texture
	{
		GrassTop = 0,
	};

	struct Vertex
	{
		uint32_t LocalPosition;
		uint32_t TextureIndex;
		uint32_t RGBI;
	};

	struct Position
	{
		uint8_t X, Y, Z;

		Position(uint8_t x, uint8_t y, uint8_t z)
			: X(x), Y(y), Z(z) {}

		bool operator == (const Position& other) const
		{
			return X == other.X && Y == other.Y && Z == other.Z;
		}
	};

}

namespace std {

	// X and Y can just be in the range of 0 - 15 (4 bit uint)
	// Z is in the range 0 - 255 (8 bit uint)
	// This means that I can hash the block position buy putting them all in a uint16
	template<>
	struct hash<Minecraft::Block::Position>
	{
		size_t operator()(const Minecraft::Block::Position& key) const
		{
			return hash<uint16_t>()(key.X + (key.Y << 4) + (key.Z << 8));
		}
	};

}
