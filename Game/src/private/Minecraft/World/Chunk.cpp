#include "mcpch.h"
#include "Minecraft/World/Chunk.h"

#include "Minecraft/World.h"
#include "Minecraft/World/WorldGen.h"


namespace Minecraft {

	struct ChunkData
	{
		static const uint32_t MaxBlocks = 16 * 16 * 256;
		static const uint32_t MaxVertices = 4 * 6 * MaxBlocks;
		static const uint32_t MaxIndices = 6 * 6 * MaxBlocks;
	};

	Chunk::Chunk(const ChunkPosition& position, World* world)
		: m_Position(position), m_World(world)
	{
		for (uint8_t x = 0; x < 16; x++)
		{
			for (uint8_t y = 0; y < 16; y++)
			{
				int worldX = 16 * m_Position.x + x;
				int worldY = 16 * m_Position.y + y;

				uint8_t height = WorldGen::GetHeight(worldX, worldY);

				// Grass block
				m_Blocks[{ x, y, height }] = Block::Create(Block::Type::Grass);
				m_Blocks[{ x, y, height - 1 }] = Block::Create(Block::Type::Dirt);
				m_Blocks[{ x, y, height - 2}] = Block::Create(Block::Type::Dirt);
				m_Blocks[{ x, y, height - 3}] = Block::Create(Block::Type::Dirt);
				m_Blocks[{ x, y, height - 4}] = Block::Create(Block::Type::Dirt);

				for (uint8_t z = 1; z < height - 4; z++)
				{
					m_Blocks[{ x, y, z }] = Block::Create(Block::Type::Stone);
				}

				// Water
				if (height < 64)
				{
					for (uint8_t z = height + 1; z <= 64; z++)
					{
						m_Blocks[{ x, y, z }] = Block::Create(Block::Type::Water);
					}
				}

				m_Blocks[{ x, y, 0 }] = Block::Create(Block::Type::Bedrock);

				// Super flat world generation
				/*
				m_Blocks[{ x, y, 6 }] = Block::Create(Block::Type::Grass);
				m_Blocks[{ x, y, 5 }] = Block::Create(Block::Type::Dirt);
				m_Blocks[{ x, y, 4 }] = Block::Create(Block::Type::Dirt);
				m_Blocks[{ x, y, 3 }] = Block::Create(Block::Type::Stone);
				m_Blocks[{ x, y, 2 }] = Block::Create(Block::Type::Stone);
				m_Blocks[{ x, y, 1 }] = Block::Create(Block::Type::Stone);
				m_Blocks[{ x, y, 0 }] = Block::Create(Block::Type::Bedrock);
				*/
			}
		}
	}

	bool Chunk::HasBlock(const LocalBlockPosition& position)
	{
		return m_Blocks.find(position) != m_Blocks.end();
	}

	bool Chunk::HasBlock(const WorldBlockPosition& position)
	{
		return m_Blocks.find(position.GetLocalPosition()) != m_Blocks.end();
	}

	Ref<Block> Chunk::GetBlock(const LocalBlockPosition& position) const
	{
		return m_Blocks.at(position);
	}

	Ref<Block> Chunk::GetBlock(const WorldBlockPosition& position) const
	{
		return m_Blocks.at(position.GetLocalPosition());
	}

	void Chunk::BreakBlock(const LocalBlockPosition& position)
	{
		m_Blocks.erase(position);
		GenerateMeshes();
	}

	void Chunk::BreakBlock(const WorldBlockPosition& position)
	{
		m_Blocks.erase(position.GetLocalPosition());
		GenerateMeshes();
	}

	void Chunk::GenerateMeshes()
	{
		// Solid
		TerrainVertex* solidTerrainVertexBufferBase = new TerrainVertex[ChunkData::MaxVertices];
		TerrainVertex* solidTerrainVertexBufferPtr = solidTerrainVertexBufferBase;

		// Transparent
		TerrainVertex* transparentTerrainVertexBufferBase = new TerrainVertex[ChunkData::MaxVertices];
		TerrainVertex* transparentTerrainVertexBufferPtr = transparentTerrainVertexBufferBase;

		// TODO: Water
		//Block::WaterVertex* waterVertexBufferBase = new Block::WaterVertex[ChunkData::MaxVertices];
		//Block::WaterVertex* waterVertexBufferPtr = waterVertexBufferBase;

		for (auto& [blockPos, block] : m_Blocks)
		{
			Block::Renderpass renderpass = block->GetRenderpass();

			WorldBlockPosition blockWorldPos = {
				m_Position.x * 16 + blockPos.x,
				m_Position.y * 16 + blockPos.y,
				(uint8_t)blockPos.z,
			};

			switch (renderpass)
			{
				case Block::Renderpass::Solid:			solidTerrainVertexBufferPtr = block->GenerateTerrainMesh(Block::Renderpass::Solid, solidTerrainVertexBufferPtr, m_World, blockWorldPos); break;
				case Block::Renderpass::Transparant:	transparentTerrainVertexBufferPtr = block->GenerateTerrainMesh(Block::Renderpass::Transparant, transparentTerrainVertexBufferPtr, m_World, blockWorldPos); break;
				//case Block::Renderpass::Water:			waterVertexBufferPtr = block->GenerateTerrainMesh(waterVertexBufferPtr, m_World, blockWorldPos); break;
			}
		}

		// -- Solid Terrain --
		uint32_t solidTerrainVertexCount = (uint32_t)(solidTerrainVertexBufferPtr - solidTerrainVertexBufferBase);
		uint32_t solidTerrainIndexCount = (uint32_t)((float)solidTerrainVertexCount * 1.5f);

		m_HasSolidTerrain = solidTerrainVertexCount > 0;
		if (m_HasSolidTerrain)
		{
			m_SolidTerrainMesh = VertexArray::Create();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(solidTerrainVertexBufferBase, solidTerrainVertexCount * sizeof(TerrainVertex));

			vertexBuffer->SetLayout({
				{ ShaderDataType::Int, "Pack1" },
				{ ShaderDataType::Int, "Color RGBV" },
			});

			m_SolidTerrainMesh->AddVertexBuffer(vertexBuffer);

			uint32_t* indices = new uint32_t[solidTerrainIndexCount];
			uint32_t offset = 0;
			for (uint32_t i = 0; i < solidTerrainIndexCount; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, solidTerrainIndexCount);
			m_SolidTerrainMesh->SetIndexBuffer(indexBuffer);
			delete[] indices;
		}

		// -- Transparent --
		uint32_t transparentTerrainVertexCount = (uint32_t)(transparentTerrainVertexBufferPtr - transparentTerrainVertexBufferBase);
		uint32_t transparentTerrainIndexCount = (uint32_t)((float)transparentTerrainVertexCount * 1.5f);

		m_HasTransparentTerrain = transparentTerrainVertexCount > 0;
		if (m_HasTransparentTerrain)
		{
			m_TransparentTerrainMesh = VertexArray::Create();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(transparentTerrainVertexBufferBase, transparentTerrainVertexCount * sizeof(TerrainVertex));

			vertexBuffer->SetLayout({
				{ ShaderDataType::Int, "Pack1" },
				{ ShaderDataType::Int, "Color RGBV" },
			});

			m_TransparentTerrainMesh->AddVertexBuffer(vertexBuffer);

			uint32_t* indices = new uint32_t[transparentTerrainIndexCount];
			uint32_t offset = 0;
			for (uint32_t i = 0; i < transparentTerrainIndexCount; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, transparentTerrainIndexCount);
			m_TransparentTerrainMesh->SetIndexBuffer(indexBuffer);
			delete[] indices;
		}

		delete[] solidTerrainVertexBufferBase;
		delete[] transparentTerrainVertexBufferBase;
	}

}
