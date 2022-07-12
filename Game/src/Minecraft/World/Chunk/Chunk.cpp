#include "mcpch.h"
#include "Minecraft/World/Chunk/Chunk.h"

// TODO: Figure out why it doesn't make sence to me with 16x16 chunks
// Or in other words, figure out a nice way to have 16x16 chunks


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
		// Generate Chunk Blocks
		//for (uint8_t x = 0; x < 16; x++)
		for (uint8_t x = 0; x < 15; x++)
		{
			//for (uint8_t y = 0; y < 16; y++)
			for (uint8_t y = 0; y < 15; y++)
			{
				for (uint8_t z = 0; z < 4; z++)
				{
					m_Blocks[{ x, y, z }] = { Block::Type::Grass, { x, y, z } };
				}
			}
		}

		m_Blocks[{ 0, 0, 0 }] = { Block::Type::Grass, { 0, 0, 0 } };

		m_VertexArray = VertexArray::Create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(ChunkData::MaxVertices * sizeof(Block::Vertex));

		vertexBuffer->SetLayout({
			{ ShaderDataType::Int, "Local Position XYZ" },
			{ ShaderDataType::Int, "Color RGBI" },
		});

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		Block::Vertex* vertexBufferBase = new Block::Vertex[ChunkData::MaxVertices];
		Block::Vertex* vertexBufferPtr = vertexBufferBase;

		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;

		//for (uint8_t x = 0; x < 16; x++)
		for (uint8_t x = 0; x < 15; x++)
		{
			//for (uint8_t y = 0; y < 16; y++)
			for (uint8_t y = 0; y < 15; y++)
			{
				//for (uint8_t z = 0; z < 256; z++)
				for (uint8_t z = 0; z < 255; z++)
				{
					// If the block does not exist
					if (m_Blocks.find({ x, y, z }) == m_Blocks.end())
					{
						// This is to prevent the uint8 from wrapping around to 0 and making an infinite loop
						//if (z == 255)
						if (z == 254)
							break;

						continue;
					}

					Block& block = m_Blocks.at({ x, y, z });
					Int3 blockPosition = block.GetPosition();

					// Look for all neighboring blocks
					// If the neighboring block does not exist, draw a face there

					// -- Top --
					if (z == 255 || m_Blocks.find(Block::Position(x, y, z + 1)) == m_Blocks.end())
					{
						vertexBufferPtr->LocalXYZ = VertexPosition(x, y, z + 1);
						vertexBufferPtr->RGBI = VertexColor(0, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x + 1, y, z + 1);
						vertexBufferPtr->RGBI = VertexColor(255, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x + 1, y + 1, z + 1);
						vertexBufferPtr->RGBI = VertexColor(255, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x, y + 1, z + 1);
						vertexBufferPtr->RGBI = VertexColor(0, 255, 0, 255);
						vertexBufferPtr++;

						vertexCount += 4;
						indexCount += 6;
					}

					// -- Bottom --
					if (m_Blocks.find(Block::Position(x, y, z - 1)) == m_Blocks.end())
					{
						vertexBufferPtr->LocalXYZ = VertexPosition(x, y + 1, z);
						vertexBufferPtr->RGBI = VertexColor(255, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x + 1, y + 1, z);
						vertexBufferPtr->RGBI = VertexColor(0, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x + 1, y, z);
						vertexBufferPtr->RGBI = VertexColor(0, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x, y, z);
						vertexBufferPtr->RGBI = VertexColor(255, 0, 0, 255);
						vertexBufferPtr++;

						vertexCount += 4;
						indexCount += 6;
					}

					// -- North --
					if (m_Blocks.find(Block::Position(x, y + 1, z)) == m_Blocks.end())
					{
						vertexBufferPtr->LocalXYZ = VertexPosition(x + 1, y + 1, z);
						vertexBufferPtr->RGBI = VertexColor(0, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x, y + 1, z);
						vertexBufferPtr->RGBI = VertexColor(255, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x, y + 1, z + 1);
						vertexBufferPtr->RGBI = VertexColor(255, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x + 1, y + 1, z + 1);
						vertexBufferPtr->RGBI = VertexColor(0, 255, 0, 255);
						vertexBufferPtr++;

						vertexCount += 4;
						indexCount += 6;
					}

					// -- South --
					if (m_Blocks.find(Block::Position(x, y - 1, z)) == m_Blocks.end())
					{
						vertexBufferPtr->LocalXYZ = VertexPosition(x, y, z);
						vertexBufferPtr->RGBI = VertexColor(0, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x + 1, y, z);
						vertexBufferPtr->RGBI = VertexColor(255, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x + 1, y, z + 1);
						vertexBufferPtr->RGBI = VertexColor(255, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x, y, z + 1);
						vertexBufferPtr->RGBI = VertexColor(0, 255, 0, 255);
						vertexBufferPtr++;

						vertexCount += 4;
						indexCount += 6;
					}

					// -- East --
					if (m_Blocks.find(Block::Position(x + 1, y, z)) == m_Blocks.end())
					{
						vertexBufferPtr->LocalXYZ = VertexPosition(x + 1, y, z);
						vertexBufferPtr->RGBI = VertexColor(0, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x + 1, y + 1, z);
						vertexBufferPtr->RGBI = VertexColor(255, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x + 1, y + 1, z + 1);
						vertexBufferPtr->RGBI = VertexColor(255, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x + 1, y, z + 1);
						vertexBufferPtr->RGBI = VertexColor(0, 255, 0, 255);
						vertexBufferPtr++;

						vertexCount += 4;
						indexCount += 6;
					}

					// -- West --
					if (m_Blocks.find(Block::Position(x - 1, y, z)) == m_Blocks.end())
					{
						vertexBufferPtr->LocalXYZ = VertexPosition(x, y + 1, z);
						vertexBufferPtr->RGBI = VertexColor(0, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x, y, z);
						vertexBufferPtr->RGBI = VertexColor(255, 0, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x, y, z + 1);
						vertexBufferPtr->RGBI = VertexColor(255, 255, 0, 255);
						vertexBufferPtr++;

						vertexBufferPtr->LocalXYZ = VertexPosition(x, y + 1, z + 1);
						vertexBufferPtr->RGBI = VertexColor(0, 255, 0, 255);
						vertexBufferPtr++;

						vertexCount += 4;
						indexCount += 6;
					}

					// This is to prevent the uint8 from wrapping around to 0 and making an infinite loop
					//if (z == 255)
					if (z == 254)
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
