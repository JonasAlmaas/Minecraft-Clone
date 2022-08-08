#pragma once

using namespace Moon;

#include "Minecraft/World/Block.h"
#include "Minecraft/World/ChunkBlock.h"


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
		Chunk(uint64_t seed, const Position& chunkPosition);

		const Position GetPosition() const { return m_ChunkPosition; }

		bool HasSolidBlocks() const { return m_HasSolidBlocks; }
		bool HasTransparentBlocks() const { return m_HasTransparentBlocks; }

		const Ref<VertexArray> GetVertexArraySolid() const { return m_VertexArraySolid; };
		const Ref<VertexArray> GetVertexArrayTransparent() const { return m_VertexArrayTransparent; };

		bool HasBlock(ChunkBlock::Position& pos) const { return m_Blocks.find(pos) != m_Blocks.end(); }
		Ref<Block> GetBlock(ChunkBlock::Position& pos) const { return m_Blocks.at(pos); }

		void BreakBlock(ChunkBlock::Position& pos);

	private:
		void GenerateVertexArray();

	private:
		uint64_t m_Seed;
		Position m_ChunkPosition;

		bool m_HasSolidBlocks;
		bool m_HasTransparentBlocks;

		Ref<VertexArray> m_VertexArraySolid;
		Ref<VertexArray> m_VertexArrayTransparent;

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
