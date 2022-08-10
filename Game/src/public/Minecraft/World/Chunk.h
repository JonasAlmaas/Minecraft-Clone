#pragma once

using namespace Moon;

#include "Minecraft/World/Block.h"


namespace Minecraft {

	class World;

	struct ChunkPosition : public Int2
	{
		ChunkPosition() : Int2() {}
		ChunkPosition(int x, int y) : Int2(x, y) {}

		using Iterator = VectorIterator<ChunkPosition>;

		bool IsWithinRadius(ChunkPosition& center, float radius)
		{
			// https://www.redblobgames.com/grids/circle-drawing/#distance-test
			float dx = (float)(center.x - x);
			float dy = (float)(center.y - y);
			float distance_squared = dx * dx + dy * dy;
			return distance_squared <= radius * radius;
		}
	};

	class Chunk
	{
	public:
		Chunk(const ChunkPosition& position, World* world);

		const ChunkPosition GetPosition() const { return m_Position; }

		bool HasBlock(const LocalBlockPosition& position);
		bool HasBlock(const WorldBlockPosition& position);

		Ref<Block> GetBlock(const LocalBlockPosition& position) const;
		Ref<Block> GetBlock(const WorldBlockPosition& position) const;

		bool HasSolidTerrain() const { return m_HasSolidTerrain; }
		bool HasTransparentTerrain() const { return m_HasTransparentTerrain; }
		bool HasWater() const { return m_HasWater; }

		void GenerateMeshes();

		const Ref<VertexArray> GetSolidTerrainMesh() const { return m_SolidTerrainMesh; }
		const Ref<VertexArray> GetTransparentTerrainMesh() const { return m_TransparentTerrainMesh; }

		void BreakBlock(const LocalBlockPosition& position);
		void BreakBlock(const WorldBlockPosition& position);

	private:
		ChunkPosition m_Position;
		World* m_World;

		bool m_HasSolidTerrain = false;
		bool m_HasTransparentTerrain = false;
		bool m_HasWater;

		Ref<VertexArray> m_SolidTerrainMesh;
		Ref<VertexArray> m_TransparentTerrainMesh;

		std::unordered_map<LocalBlockPosition, Ref<Block>> m_Blocks;

	};

}

namespace std {

	template<>
	struct hash<Minecraft::ChunkPosition>
	{
		size_t operator()(const Minecraft::ChunkPosition& key) const
		{
			return hash<size_t>()(key.x | ((unsigned long long)key.y) << 32);
		}
	};

}
