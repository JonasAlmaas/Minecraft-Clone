#include "mcpch.h"
#include "Minecraft/World/World.h"


namespace Minecraft {

	struct BlockVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	World::World(uint64_t seed)
		: m_Seed(seed)
	{
		m_VA = VertexArray::Create();

		//float vertices[4 * 6 * sizeof(BlockVertex)] = {
		float vertices[4 * sizeof(BlockVertex)] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		};

		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices));

		vb->SetLayout({
			{ ShaderDataType::Float3, "Position XYZ" },
			{ ShaderDataType::Float4, "Color RGBA" },
		});

		m_VA->AddVertexBuffer(vb);


		uint32_t indices[6] = {
			0, 1, 2,
			0, 2, 3,
		};

		Ref<IndexBuffer> ib = IndexBuffer::Create(indices, 6);
		m_VA->SetIndexBuffer(ib);
	}

}
