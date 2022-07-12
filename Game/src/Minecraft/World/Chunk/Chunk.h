#pragma once

using namespace Moon;

#include "Minecraft/World/Block/Block.h"


namespace Minecraft {

	class Chunk
	{
	public:
		struct VertexPosition
		{
			// X and Y are 4 bit each
			// Z is 8 bit
			uint8_t BlockX, BlockY, BlockZ;

			bool OffsetX, OffsetY, OffsetZ;

			VertexPosition(uint8_t blockX, uint8_t blockY, uint8_t blockZ, bool offsetX, bool offsetY, bool offsetZ)
				: BlockX(blockX), BlockY(blockY), BlockZ(blockZ), OffsetX(offsetX), OffsetY(offsetY), OffsetZ(offsetZ) {}

			operator uint32_t () { return BlockX + (BlockY << 4) + (BlockZ << 8) + (OffsetX << 16) + (OffsetY << 17) + (OffsetZ << 18); }
		};

		struct VertexColor
		{
			uint8_t R, G, B, I;

			VertexColor(uint8_t r, uint8_t g, uint8_t b, uint8_t i)
				: R(r), G(g), B(b), I(i) {}

			operator uint32_t () { return R + (G << 8) + (B << 16) + (I << 24); }
		};

	public:
		Chunk(const Int2& chunkPosition);

		void GenerateBlocks();
		void GenerateVertexArray();

		const Int2 GetPosition() const { return m_ChunkPosition; }
		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; };

	private:
		Int2 m_ChunkPosition;

		std::unordered_map<Block::Position, Block::Type> m_Blocks;

		Ref<VertexArray> m_VertexArray;

	};

}
