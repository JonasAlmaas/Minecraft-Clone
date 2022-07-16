#pragma once

using namespace Moon;

#include "Minecraft/World/Block/Block.h"
#include "Minecraft/World/Block/ChunkBlock.h"


namespace Minecraft {

	class Chunk
	{
	public:
		struct Position : public Int2
		{
		public:
			Position() : Int2() {}
			Position(int x, int y) : Int2(x, y) {}

			bool IsWithinRadius(Position& center, float radius)
			{
				// https://www.redblobgames.com/grids/circle-drawing/#distance-test
				float dx = (float)(center.x - x);
				float dy = (float)(center.y - y);
				float distance_squared = dx * dx + dy * dy;
				return distance_squared <= radius * radius;
			}
		};

		struct PositionIterator
		{
		public:
			PositionIterator(Position* ptr) : m_Ptr(ptr) {}

			Position& operator*() const { return *m_Ptr; }
			Position* operator->() { return m_Ptr; }

			// Prefix increment
			PositionIterator& operator++() { m_Ptr++; return *this; }

			// Postfix increment
			PositionIterator operator++(int) { PositionIterator tmp = *this; ++(*this); return tmp; }

			friend bool operator== (const PositionIterator& a, const PositionIterator& b) { return a.m_Ptr == b.m_Ptr; };
			friend bool operator!= (const PositionIterator& a, const PositionIterator& b) { return a.m_Ptr != b.m_Ptr; };

		private:
			Position* m_Ptr;
		};

		struct VertexPosition
		{
			uint8_t BlockX, BlockY, BlockZ;
			bool OffsetX, OffsetY, OffsetZ;
			bool U, V;

			operator uint32_t () { return BlockX + (BlockY << 4) + (BlockZ << 8) + (OffsetX << 16) + (OffsetY << 17) + (OffsetZ << 18) + (U << 19) + (V << 20); }
		};

		struct VertexColor
		{
			uint8_t R, G, B, V;

			operator uint32_t () { return R + (G << 8) + (B << 16) + (V << 24); }
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
			return hash<size_t>()(key.x | ((unsigned long long)key.y) << 32);
		}
	};

}
