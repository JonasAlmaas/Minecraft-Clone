#pragma once

using namespace Moon;

#include "Minecraft/World/Block/Block.h"
#include "Minecraft/World/Block/ChunkBlock.h"


namespace Minecraft {

	class Chunk
	{
	public:
		struct VertexPosition
		{
			uint8_t BlockX, BlockY, BlockZ;
			bool OffsetX, OffsetY, OffsetZ;
			bool U, V;

			VertexPosition(uint8_t blockX, uint8_t blockY, uint8_t blockZ, bool offsetX, bool offsetY, bool offsetZ, bool u, bool v)
				: BlockX(blockX), BlockY(blockY), BlockZ(blockZ), OffsetX(offsetX), OffsetY(offsetY), OffsetZ(offsetZ), U(u), V(v) {}

			operator uint32_t () { return BlockX + (BlockY << 4) + (BlockZ << 8) + (OffsetX << 16) + (OffsetY << 17) + (OffsetZ << 18) + (U << 19) + (V << 20); }
		};

		struct VertexColor
		{
			uint8_t R, G, B, V;

			VertexColor(uint8_t r, uint8_t g, uint8_t b, uint8_t v)
				: R(r), G(g), B(b), V(v) {}

			operator uint32_t () { return R + (G << 8) + (B << 16) + (V << 24); }
		};

		struct Position
		{
			int32_t X, Y;

			Position(int32_t x, int32_t y)
				: X(x), Y(y) {}

			bool operator == (const Position& other) const
			{
				return X == other.X && Y == other.Y;
			}
		};

	public:
		Chunk(const Position& chunkPosition);

		void GenerateBlocks();
		void GenerateVertexArray();

		const Position GetPosition() const { return m_ChunkPosition; }
		const Ref<VertexArray> GetVertexArray() const { return m_VertexArray; };

	private:
		Position m_ChunkPosition;
		Ref<VertexArray> m_VertexArray;
		std::unordered_map<ChunkBlock::Position, Ref<Block>> m_Blocks;

	};


}
namespace std {

	template<>
	struct hash<Minecraft::Chunk::Position>
	{
		size_t operator()(const Minecraft::Chunk::Position& key) const
		{
			return hash<int>()(key.X + (key.Y << 32));
		}
	};

}
