#include "mcpch.h"
#include "Minecraft/World/Chunk/Chunk.h"


namespace Minecraft {

	struct ChunkData
	{
		static const uint32_t MaxBlocks = 16 * 16 * 256;
		static const uint32_t MaxVertices = 4 * 6 * MaxBlocks;
		static const uint32_t MaxIndices = 6 * 6 * MaxBlocks;
	};

	Chunk::Chunk(const Position& chunkPosition)
		: m_ChunkPosition(chunkPosition)
	{
		GenerateBlocks();
		GenerateVertexArray();
	}

	void Chunk::GenerateBlocks()
	{
		// Generate Chunk Blocks
		for (uint8_t x = 0; x < 16; x++)
		{
			for (uint8_t y = 0; y < 16; y++)
			{
				m_Blocks[{ x, y, 6 }] = Block::Create(Block::Type::Grass);
				m_Blocks[{ x, y, 5 }] = Block::Create(Block::Type::Dirt);
				m_Blocks[{ x, y, 4 }] = Block::Create(Block::Type::Dirt);
				m_Blocks[{ x, y, 3 }] = Block::Create(Block::Type::Stone);
				m_Blocks[{ x, y, 2 }] = Block::Create(Block::Type::Stone);
				m_Blocks[{ x, y, 1 }] = Block::Create(Block::Type::Stone);
				m_Blocks[{ x, y, 0 }] = Block::Create(Block::Type::Bedrock);
			}
		}
	}

	void Chunk::GenerateVertexArray()
	{
		ChunkBlock::Vertex* vertexBufferBase = new ChunkBlock::Vertex[ChunkData::MaxVertices];
		ChunkBlock::Vertex* vertexBufferPtr = vertexBufferBase;

		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;

		for (auto& [pos, block] : m_Blocks)
		{
			// -- Top --
			if (pos.z == 255 || m_Blocks.find(ChunkBlock::Position(pos.x, pos.y, pos.z + 1)) == m_Blocks.end())
			{
				uint16_t textureIndex = block->GetTextureIndex(Block::Face::Top);
				uint32_t color = VertexColor(0, 0, 0, 0);

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 1, 0, 0);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 1, 1, 0);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 1, 1, 1);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 1, 0, 1);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexCount += 4;
				indexCount += 6;
			}

			// -- Bottom --
			if (pos.z == 0 || m_Blocks.find(ChunkBlock::Position(pos.x, pos.y, pos.z - 1)) == m_Blocks.end())
			{
				uint16_t textureIndex = block->GetTextureIndex(Block::Face::Bottom);
				uint32_t color = VertexColor(0, 0, 0, 0);

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 0, 1, 1);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 0, 0, 1);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 0, 0, 0);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 0, 1, 0);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexCount += 4;
				indexCount += 6;
			}
			// -- North --
			if (m_Blocks.find(ChunkBlock::Position(pos.x, pos.y + 1, pos.z)) == m_Blocks.end())
			{
				uint16_t textureIndex = block->GetTextureIndex(Block::Face::North);
				uint32_t color = VertexColor(0, 0, 0, 0);

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 0, 0, 0);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 0, 1, 0);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 1, 1, 1);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 1, 0, 1);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexCount += 4;
				indexCount += 6;
			}


			// -- South --
			if (pos.y == 0 || m_Blocks.find(ChunkBlock::Position(pos.x, pos.y - 1, pos.z)) == m_Blocks.end())
			{
				uint16_t textureIndex = block->GetTextureIndex(Block::Face::South);
				uint32_t color = VertexColor(0, 0, 0, 0);

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 0, 0, 0);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 0, 1, 0);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 1, 1, 1);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 1, 0, 1);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexCount += 4;
				indexCount += 6;
			}

			// -- East --
			if (m_Blocks.find(ChunkBlock::Position(pos.x + 1, pos.y, pos.z)) == m_Blocks.end())
			{
				uint16_t textureIndex = block->GetTextureIndex(Block::Face::East);
				uint32_t color = VertexColor(0, 0, 0, 0);

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 0, 0, 0);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 0, 1, 0);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 1, 1, 1);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 1, 0, 1);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexCount += 4;
				indexCount += 6;
			}

			// -- West --
			if (pos.x == 0 || m_Blocks.find(ChunkBlock::Position(pos.x - 1, pos.y, pos.z)) == m_Blocks.end())
			{
				uint16_t textureIndex = block->GetTextureIndex(Block::Face::West);
				uint32_t color = VertexColor(0, 0, 0, 0);

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 0, 0, 0);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 0, 1, 0);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 1, 1, 1);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexBufferPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 1, 0, 1);
				vertexBufferPtr->TextureIndex = textureIndex;
				vertexBufferPtr->RGBV = color;
				vertexBufferPtr++;

				vertexCount += 4;
				indexCount += 6;
			}
		}

		if (vertexCount > 0)
		{
			m_VertexArray = VertexArray::Create();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(ChunkData::MaxVertices * sizeof(ChunkBlock::Vertex));

			vertexBuffer->SetLayout({
				{ ShaderDataType::Int, "Packed Local Position" },
				{ ShaderDataType::Int, "Texture Index" },
				{ ShaderDataType::Int, "Color RGBV" },
				});

			m_VertexArray->AddVertexBuffer(vertexBuffer);
		
			vertexBuffer->SetData(vertexBufferBase, vertexCount * sizeof(ChunkBlock::Vertex));
			delete[] vertexBufferBase;

			uint32_t* indices = new uint32_t[indexCount];
			uint32_t offset = 0;
			for (uint32_t i = 0; i < indexCount; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, indexCount);
			m_VertexArray->SetIndexBuffer(indexBuffer);
			delete[] indices;
		}
	}

}
