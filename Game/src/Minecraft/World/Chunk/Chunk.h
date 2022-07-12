#pragma once

using namespace Moon;

#include "Minecraft/World/Block/Block.h"


namespace Minecraft {

	class Chunk
	{
	public:
		Chunk(const Int2& chunkPosition);

		const Int2 GetPosition() const { return m_ChunkPosition; }
		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; };

	private:
		Int2 m_ChunkPosition;

		std::unordered_map<Block::Position, Block> m_Blocks;

		Ref<VertexArray> m_VertexArray;

	};

}
