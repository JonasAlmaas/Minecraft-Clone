#include "mcpch.h"
#include "Minecraft/Renderer/GameRenderer.h"

#include "Minecraft/World/Chunk/Chunk.h"


namespace Minecraft {

	struct RendererData
	{
		Ref<Shader> BlockShader;
		Ref<Texture2D> BlockTextureAtlas;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;

		struct ChunkPositionData
		{
			int32_t X, Y;
		};
		ChunkPositionData ChunkPositionBuffer;
		Ref<UniformBuffer> ChunkPositionUniformBuffer;
	};

	static RendererData* s_Data = nullptr;

	void GameRenderer::Init()
	{
		s_Data = new RendererData();

		s_Data->BlockShader = Shader::Create("Content/Shaders/Minecraft/Chunk.glsl");

		Texture2DSpecification spec;
		spec.MinFilter = Texture2DFilter::Nearest;
		spec.MagFilter = Texture2DFilter::Nearest;
		spec.WrapS = Texture2DWrap::ClampToEdge;
		spec.WrapT = Texture2DWrap::ClampToEdge;
		s_Data->BlockTextureAtlas = Texture2D::Create("Content/Textures/Atlases/terrain-atlas.png", spec);

		s_Data->CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);
		s_Data->ChunkPositionUniformBuffer = UniformBuffer::Create(sizeof(RendererData::ChunkPositionData), 1);

		RenderCommand::SetMultisample(false);
	}

	void GameRenderer::Shutdown()
	{
		delete s_Data;
		s_Data = nullptr;
	}

	void GameRenderer::ReloadShaders()
	{
		s_Data->BlockShader->Reload();
	}

	void GameRenderer::RenderWorld(const Ref<World>& world, const glm::mat4& viewProjectionMatrix)
	{
		RenderCommand::SetDepthTest(true);

		s_Data->CameraBuffer.ViewProjection = viewProjectionMatrix;
		s_Data->CameraUniformBuffer->SetData(&s_Data->CameraBuffer, sizeof(RendererData::CameraData));

		s_Data->BlockTextureAtlas->Bind();
		s_Data->BlockShader->Bind();

		for (auto& chunkPosition : *world)
		{
			if (world->HasChunk(chunkPosition))
			{
				s_Data->ChunkPositionBuffer.X = chunkPosition.x;
				s_Data->ChunkPositionBuffer.Y = chunkPosition.y;
				s_Data->ChunkPositionUniformBuffer->SetData(&s_Data->ChunkPositionBuffer, sizeof(RendererData::ChunkPositionData));

				RenderCommand::DrawIndexed(world->GetChunk(chunkPosition)->GetVertexArray());
			}
		}
	}

}
