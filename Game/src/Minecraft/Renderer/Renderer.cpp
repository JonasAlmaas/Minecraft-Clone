#include "mcpch.h"
#include "Minecraft/Renderer/Renderer.h"


namespace Minecraft {

	struct RendererData
	{
		Ref<Shader> BlockShader;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;

		struct ChunkPositionData
		{
			uint32_t X, Y;
		};
		ChunkPositionData ChunkPositionBuffer;
		Ref<UniformBuffer> ChunkPositionUniformBuffer;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		s_Data.BlockShader = Shader::Create("Content/Shaders/Minecraft/Chunk.glsl");

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);
		s_Data.ChunkPositionUniformBuffer = UniformBuffer::Create(sizeof(RendererData::ChunkPositionData), 1);
	}

	void Renderer::RenderWorld(const Ref<World>& world, const glm::mat4& viewProjectionMatrix)
	{
		s_Data.CameraBuffer.ViewProjection = viewProjectionMatrix;
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));

		s_Data.BlockShader->Bind();

		for (auto& chunk : world->m_Chunks)
		{
			Int2 chunkPos = chunk->GetPosition();

			s_Data.ChunkPositionBuffer.X = chunkPos.X;
			s_Data.ChunkPositionBuffer.Y = chunkPos.Y;

			s_Data.ChunkPositionUniformBuffer->SetData(&s_Data.ChunkPositionBuffer, sizeof(RendererData::ChunkPositionData));

			RenderCommand::DrawIndexed(chunk->GetVertexArray());
		}
	}

}
