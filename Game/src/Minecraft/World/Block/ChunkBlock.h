#pragma once

#include "mcpch.h"


namespace Minecraft::ChunkBlock {

	struct Vertex
	{
		uint32_t LocalPosition;
		uint32_t TextureIndex;
		uint32_t RGBV;
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
	struct hash<Minecraft::ChunkBlock::Position>
	{
		size_t operator()(const Minecraft::ChunkBlock::Position& key) const
		{
			return hash<uint16_t>()(key.X + (key.Y << 4) + (key.Z << 8));
		}
	};

}
