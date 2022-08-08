#include "mcpch.h"
#include "Minecraft/World/Chunk.h"


namespace Minecraft {

	struct ChunkData
	{
		static const uint32_t MaxBlocks = 16 * 16 * 256;
		static const uint32_t MaxVertices = 4 * 6 * MaxBlocks;
		static const uint32_t MaxIndices = 6 * 6 * MaxBlocks;
	};

	Chunk::Chunk(uint64_t seed, const Position& chunkPosition)
		: m_Seed(seed), m_ChunkPosition(chunkPosition)
	{
		Noise::Specification noiseSpec;
		noiseSpec.Seed = m_Seed;
		Noise noise = Noise(noiseSpec);

		// Generate Chunk Blocks
		for (uint8_t x = 0; x < 16; x++)
		{
			for (uint8_t y = 0; y < 16; y++)
			{
				int64_t worldX = m_ChunkPosition.x * 16 + x;
				int64_t worldY = m_ChunkPosition.y * 16 + y;
				uint8_t height = (uint8_t)((noise.Get(worldX, worldY) * 0.5 + 0.5) * 64.0f) + 40;

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

				// WATER
				if (height < 64)
				{
					for (uint8_t z = height + 1; z <= 64; z++)
					{
						m_Blocks[{ x, y, z }] = Block::Create(Block::Type::Water);
					}
				}

				m_Blocks[{ x, y, 0 }] = Block::Create(Block::Type::Bedrock);

				// Super flat world generation
				//m_Blocks[{ x, y, 5 }] = Block::Create(Block::Type::Dirt);
				//m_Blocks[{ x, y, 4 }] = Block::Create(Block::Type::Dirt);
				//m_Blocks[{ x, y, 3 }] = Block::Create(Block::Type::Stone);
				//m_Blocks[{ x, y, 2 }] = Block::Create(Block::Type::Stone);
				//m_Blocks[{ x, y, 1 }] = Block::Create(Block::Type::Stone);
				//m_Blocks[{ x, y, 0 }] = Block::Create(Block::Type::Bedrock);
			}
		}

		GenerateVertexArray();
	}

	void Chunk::BreakBlock(ChunkBlock::Position& pos)
	{
		m_Blocks.erase(pos);
		GenerateVertexArray();
	}

	void Chunk::GenerateVertexArray()
	{
		ChunkBlock::Vertex* vertexBufferSolidBase = new ChunkBlock::Vertex[ChunkData::MaxVertices];
		ChunkBlock::Vertex* vertexBufferSolidPtr = vertexBufferSolidBase;

		uint32_t solidVertexCount = 0;
		uint32_t solidIndexCount = 0;

		ChunkBlock::Vertex* vertexBufferTransparentBase = new ChunkBlock::Vertex[ChunkData::MaxVertices];
		ChunkBlock::Vertex* vertexBufferTransparentPtr = vertexBufferTransparentBase;

		uint32_t transparentVertexCount = 0;
		uint32_t transparentIndexCount = 0;

		for (auto& [pos, block] : m_Blocks)
		{
			Block::Renderpass renderpass = block->GetRenderpass();

			switch (renderpass)
			{
				case Block::Renderpass::Solid:
				{
					// -- Top --
					{
						bool hasNeighborBlock = m_Blocks.find(ChunkBlock::Position(pos.x, pos.y, pos.z + 1)) != m_Blocks.end();
						bool hasTransparentNeighbor = false;

						// Check if block is transparent
						if (hasNeighborBlock)
						{
							Ref<Block> neighborBlock = m_Blocks.at(ChunkBlock::Position(pos.x, pos.y, pos.z + 1));
							if (neighborBlock->GetRenderpass() == Block::Renderpass::Transparant)
								hasTransparentNeighbor = true;
						}

						if (pos.z == 255 || !hasNeighborBlock || hasTransparentNeighbor)
						{
							uint16_t textureIndex = block->GetTextureIndex(Block::Face::Top);
							uint32_t color = VertexColor(0, 0, 0, 0);

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 1, 0, 0);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 1, 1, 0);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 1, 1, 1);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 1, 0, 1);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							solidVertexCount += 4;
							solidIndexCount += 6;
						}
					}

					// -- Bottom --
					{
						bool hasNeighborBlock = m_Blocks.find(ChunkBlock::Position(pos.x, pos.y, pos.z - 1)) != m_Blocks.end();
						bool hasTransparentNeighbor = false;

						// Check if block is transparent
						if (hasNeighborBlock)
						{
							Ref<Block> neighborBlock = m_Blocks.at(ChunkBlock::Position(pos.x, pos.y, pos.z - 1));
							if (neighborBlock->GetRenderpass() == Block::Renderpass::Transparant)
								hasTransparentNeighbor = true;
						}

						if (pos.z == 0 || !hasNeighborBlock || hasTransparentNeighbor)
						{
							uint16_t textureIndex = block->GetTextureIndex(Block::Face::Bottom);
							uint32_t color = VertexColor(0, 0, 0, 0);

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 0, 1, 1);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 0, 0, 1);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 0, 0, 0);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 0, 1, 0);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							solidVertexCount += 4;
							solidIndexCount += 6;
						}
					}

					// -- North --
					{
						bool hasNeighborBlock = m_Blocks.find(ChunkBlock::Position(pos.x, pos.y + 1, pos.z)) != m_Blocks.end();
						bool hasTransparentNeighbor = false;

						// Check if block is transparent
						if (hasNeighborBlock)
						{
							Ref<Block> neighborBlock = m_Blocks.at(ChunkBlock::Position(pos.x, pos.y + 1, pos.z));
							if (neighborBlock->GetRenderpass() == Block::Renderpass::Transparant)
								hasTransparentNeighbor = true;
						}

						if (!hasNeighborBlock || hasTransparentNeighbor)
						{
							uint16_t textureIndex = block->GetTextureIndex(Block::Face::North);
							uint32_t color = VertexColor(0, 0, 0, 0);

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 0, 0, 0);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 0, 1, 0);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 1, 1, 1);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 1, 0, 1);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							solidVertexCount += 4;
							solidIndexCount += 6;
						}
					}

					// -- South --
					{
						bool hasNeighborBlock = m_Blocks.find(ChunkBlock::Position(pos.x, pos.y - 1, pos.z)) != m_Blocks.end();
						bool hasTransparentNeighbor = false;

						// Check if block is transparent
						if (hasNeighborBlock)
						{
							Ref<Block> neighborBlock = m_Blocks.at(ChunkBlock::Position(pos.x, pos.y - 1, pos.z));
							if (neighborBlock->GetRenderpass() == Block::Renderpass::Transparant)
								hasTransparentNeighbor = true;
						}

						if (pos.y == 0 || !hasNeighborBlock || hasTransparentNeighbor)
						{
							uint16_t textureIndex = block->GetTextureIndex(Block::Face::South);
							uint32_t color = VertexColor(0, 0, 0, 0);

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 0, 0, 0);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 0, 1, 0);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 1, 1, 1);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 1, 0, 1);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							solidVertexCount += 4;
							solidIndexCount += 6;
						}
					}

					// -- East --
					{
						bool hasNeighborBlock = m_Blocks.find(ChunkBlock::Position(pos.x + 1, pos.y, pos.z)) != m_Blocks.end();
						bool hasTransparentNeighbor = false;

						// Check if block is transparent
						if (hasNeighborBlock)
						{
							Ref<Block> neighborBlock = m_Blocks.at(ChunkBlock::Position(pos.x + 1, pos.y, pos.z));
							if (neighborBlock->GetRenderpass() == Block::Renderpass::Transparant)
								hasTransparentNeighbor = true;
						}

						if (!hasNeighborBlock || hasTransparentNeighbor)
						{
							uint16_t textureIndex = block->GetTextureIndex(Block::Face::East);
							uint32_t color = VertexColor(0, 0, 0, 0);

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 0, 0, 0);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 0, 1, 0);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 1, 1, 1);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 1, 0, 1);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							solidVertexCount += 4;
							solidIndexCount += 6;
						}
					}

					// -- West --
					{
						bool hasNeighborBlock = m_Blocks.find(ChunkBlock::Position(pos.x - 1, pos.y, pos.z)) != m_Blocks.end();
						bool hasTransparentNeighbor = false;

						// Check if block is transparent
						if (hasNeighborBlock)
						{
							Ref<Block> neighborBlock = m_Blocks.at(ChunkBlock::Position(pos.x - 1, pos.y, pos.z));
							if (neighborBlock->GetRenderpass() == Block::Renderpass::Transparant)
								hasTransparentNeighbor = true;
						}

						if (pos.x == 0 || !hasNeighborBlock || hasTransparentNeighbor)
						{
							uint16_t textureIndex = block->GetTextureIndex(Block::Face::West);
							uint32_t color = VertexColor(0, 0, 0, 0);

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 0, 0, 0);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 0, 1, 0);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 1, 1, 1);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							vertexBufferSolidPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 1, 0, 1);
							vertexBufferSolidPtr->TextureIndex = textureIndex;
							vertexBufferSolidPtr->RGBV = color;
							vertexBufferSolidPtr++;

							solidVertexCount += 4;
							solidIndexCount += 6;
						}
					}

					break;
				}

				case Block::Renderpass::Transparant:
				{
					// -- Top --
					if (pos.z == 255 || m_Blocks.find(ChunkBlock::Position(pos.x, pos.y, pos.z + 1)) == m_Blocks.end())
					{
						uint16_t textureIndex = block->GetTextureIndex(Block::Face::Top);
						uint32_t color = VertexColor(0, 0, 0, 0);

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 1, 0, 0);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 1, 1, 0);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 1, 1, 1);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 1, 0, 1);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						transparentVertexCount += 4;
						transparentIndexCount += 6;
					}

					// -- Bottom --
					if (pos.z == 0 || m_Blocks.find(ChunkBlock::Position(pos.x, pos.y, pos.z - 1)) == m_Blocks.end())
					{
						uint16_t textureIndex = block->GetTextureIndex(Block::Face::Bottom);
						uint32_t color = VertexColor(0, 0, 0, 0);

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 0, 1, 1);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 0, 0, 1);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 0, 0, 0);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 0, 1, 0);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						transparentVertexCount += 4;
						transparentIndexCount += 6;
					}

					// -- North --
					if (m_Blocks.find(ChunkBlock::Position(pos.x, pos.y + 1, pos.z)) == m_Blocks.end())
					{
						uint16_t textureIndex = block->GetTextureIndex(Block::Face::North);
						uint32_t color = VertexColor(0, 0, 0, 0);

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 0, 0, 0);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 0, 1, 0);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 1, 1, 1);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 1, 0, 1);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						transparentVertexCount += 4;
						transparentIndexCount += 6;
					}

					// -- South --
					if (pos.y == 0 || m_Blocks.find(ChunkBlock::Position(pos.x, pos.y - 1, pos.z)) == m_Blocks.end())
					{
						uint16_t textureIndex = block->GetTextureIndex(Block::Face::South);
						uint32_t color = VertexColor(0, 0, 0, 0);

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 0, 0, 0);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 0, 1, 0);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 1, 1, 1);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 1, 0, 1);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						transparentVertexCount += 4;
						transparentIndexCount += 6;
					}

					// -- East --
					if (m_Blocks.find(ChunkBlock::Position(pos.x + 1, pos.y, pos.z)) == m_Blocks.end())
					{
						uint16_t textureIndex = block->GetTextureIndex(Block::Face::East);
						uint32_t color = VertexColor(0, 0, 0, 0);

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 0, 0, 0);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 0, 1, 0);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 1, 1, 1, 1);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 1, 0, 1, 0, 1);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						transparentVertexCount += 4;
						transparentIndexCount += 6;
					}

					// -- West --
					if (pos.x == 0 || m_Blocks.find(ChunkBlock::Position(pos.x - 1, pos.y, pos.z)) == m_Blocks.end())
					{
						uint16_t textureIndex = block->GetTextureIndex(Block::Face::West);
						uint32_t color = VertexColor(0, 0, 0, 0);

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 0, 0, 0);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 0, 1, 0);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 0, 1, 1, 1);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						vertexBufferTransparentPtr->LocalPosition = VertexPosition(pos.x, pos.y, pos.z, 0, 1, 1, 0, 1);
						vertexBufferTransparentPtr->TextureIndex = textureIndex;
						vertexBufferTransparentPtr->RGBV = color;
						vertexBufferTransparentPtr++;

						transparentVertexCount += 4;
						transparentIndexCount += 6;
					}

					break;
				}
			}
		}

		// -- Solid --
		if (solidVertexCount > 0)
		{
			m_HasSolidBlocks = true;

			m_VertexArraySolid = VertexArray::Create();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(ChunkData::MaxVertices * sizeof(ChunkBlock::Vertex));

			vertexBuffer->SetLayout({
				{ ShaderDataType::Int, "Packed Local Position" },
				{ ShaderDataType::Int, "Texture Index" },
				{ ShaderDataType::Int, "Color RGBV" },
				});

			m_VertexArraySolid->AddVertexBuffer(vertexBuffer);
		
			vertexBuffer->SetData(vertexBufferSolidBase, solidVertexCount * sizeof(ChunkBlock::Vertex));
			delete[] vertexBufferSolidBase;

			uint32_t* indices = new uint32_t[solidIndexCount];
			uint32_t offset = 0;
			for (uint32_t i = 0; i < solidIndexCount; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, solidIndexCount);
			m_VertexArraySolid->SetIndexBuffer(indexBuffer);
			delete[] indices;
		}
		else
		{
			m_HasSolidBlocks = false;
		}

		// -- Transparent --
		if (transparentVertexCount > 0)
		{
			m_HasTransparentBlocks = true;

			m_VertexArrayTransparent = VertexArray::Create();
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(ChunkData::MaxVertices * sizeof(ChunkBlock::Vertex));

			vertexBuffer->SetLayout({
				{ ShaderDataType::Int, "Packed Local Position" },
				{ ShaderDataType::Int, "Texture Index" },
				{ ShaderDataType::Int, "Color RGBV" },
				});

			m_VertexArrayTransparent->AddVertexBuffer(vertexBuffer);

			vertexBuffer->SetData(vertexBufferTransparentBase, transparentVertexCount * sizeof(ChunkBlock::Vertex));
			delete[] vertexBufferTransparentBase;

			uint32_t* indices = new uint32_t[transparentIndexCount];
			uint32_t offset = 0;
			for (uint32_t i = 0; i < transparentIndexCount; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, transparentIndexCount);
			m_VertexArrayTransparent->SetIndexBuffer(indexBuffer);
			delete[] indices;
		}
		else
		{
			m_HasTransparentBlocks = false;
		}
	}

}
