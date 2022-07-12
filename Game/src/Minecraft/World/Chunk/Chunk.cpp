#include "mcpch.h"
#include "Minecraft/World/Chunk/Chunk.h"


namespace Minecraft {

	struct ChunkData
	{
		static const uint32_t MaxBlocks = 16 * 16 * 256;
		static const uint32_t MaxVertices = 4 * 6 * MaxBlocks;
		static const uint32_t MaxIndices = 6 * 6 * MaxBlocks;
	};

	Chunk::Chunk(const Int2& chunkPosition)
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
				m_Blocks[{ x, y, 5 }] = Block::Type::Grass;
				m_Blocks[{ x, y, 4 }] = Block::Type::Dirt;
				m_Blocks[{ x, y, 3 }] = Block::Type::Dirt;
				m_Blocks[{ x, y, 2 }] = Block::Type::Stone;
				m_Blocks[{ x, y, 1 }] = Block::Type::Stone;
				m_Blocks[{ x, y, 0 }] = Block::Type::Stone;
			}
		}
	}

	void Chunk::GenerateVertexArray()
	{
		m_VertexArray = VertexArray::Create();
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(ChunkData::MaxVertices * sizeof(Block::Vertex));

		vertexBuffer->SetLayout({
			{ ShaderDataType::Int, "Packed Local Position" },
			{ ShaderDataType::Int, "Texture Index" },
			{ ShaderDataType::Int, "Color RGBI" },
		});

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		Block::Vertex* vertexBufferBase = new Block::Vertex[ChunkData::MaxVertices];
		Block::Vertex* vertexBufferPtr = vertexBufferBase;

		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;

		for (uint8_t x = 0; x < 16; x++)
		{
			for (uint8_t y = 0; y < 16; y++)
			{
				for (uint8_t z = 0; z < 256; z++)
				{
					// If the block does not exist
					if (m_Blocks.find({ x, y, z }) == m_Blocks.end())
					{
						// This is to prevent the uint8 from wrapping around to 0 and making an infinite loop
						if (z == 255)
							break;

						continue;
					}

					//Block::Type blockType = m_Blocks.at({ x, y, z });

					Block::Texture texture = Block::Texture::GrassTop;
					uint32_t textureIndex = (uint32_t)texture;

					// Look for all neighboring blocks
					// If the neighboring block does not exist, draw a face there

					// -- Top --
					if (z == 255 || m_Blocks.find(Block::Position(x, y, z + 1)) == m_Blocks.end())
					{
						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 0, 0, 1, 0, 0);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(0, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 1, 0, 1, 1, 0);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(255, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 1, 1, 1, 1, 1);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(255, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 0, 1, 1, 0, 1);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(0, 255, 0, 255);
						vertexBufferPtr++;

						vertexCount += 4;
						indexCount += 6;
					}

					// -- Bottom --
					if (m_Blocks.find(Block::Position(x, y, z - 1)) == m_Blocks.end())
					{
						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 0, 1, 0, 1, 1);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(255, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 1, 1, 0, 0, 1);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(0, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 1, 0, 0, 0, 0);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(0, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 0, 0, 0, 1, 0);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(255, 0, 0, 255);
						vertexBufferPtr++;

						vertexCount += 4;
						indexCount += 6;
					}
					// -- North --
					if (m_Blocks.find(Block::Position(x, y + 1, z)) == m_Blocks.end())
					{
						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 1, 1, 0, 0, 0);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(0, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 0, 1, 0, 1, 0);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(255, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 0, 1, 1, 1, 1);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(255, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 1, 1, 1, 0, 1);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(0, 255, 0, 255);
						vertexBufferPtr++;

						vertexCount += 4;
						indexCount += 6;
					}


					// -- South --
					if (m_Blocks.find(Block::Position(x, y - 1, z)) == m_Blocks.end())
					{
						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 0, 0, 0, 0, 0);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(0, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 1, 0, 0, 1, 0);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(255, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 1, 0, 1, 1, 1);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(255, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 0, 0, 1, 0, 1);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(0, 255, 0, 255);
						vertexBufferPtr++;

						vertexCount += 4;
						indexCount += 6;
					}

					// -- East --
					if (m_Blocks.find(Block::Position(x + 1, y, z)) == m_Blocks.end())
					{
						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 1, 0, 0, 0, 0);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(0, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 1, 1, 0, 1, 0);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(255, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 1, 1, 1, 1, 1);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(255, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 1, 0, 1, 0, 1);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(0, 255, 0, 255);
						vertexBufferPtr++;

						vertexCount += 4;
						indexCount += 6;
					}

					// -- West --
					if (m_Blocks.find(Block::Position(x - 1, y, z)) == m_Blocks.end())
					{
						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 0, 1, 0, 0, 0);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(0, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 0, 0, 0, 1, 0);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(255, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 0, 0, 1, 1, 1);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(255, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalPosition = VertexPosition(x, y, z, 0, 1, 1, 0, 1);
						vertexBufferPtr->TextureIndex = textureIndex;
						vertexBufferPtr->RGBI = VertexColor(0, 255, 0, 255);
						vertexBufferPtr++;

						vertexCount += 4;
						indexCount += 6;
					}

					// This is to prevent the uint8 from wrapping around to 0 and making an infinite loop
					if (z == 255)
						break;
				}
			}
		}

		vertexBuffer->SetData(vertexBufferBase, vertexCount * sizeof(Block::Vertex));
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
