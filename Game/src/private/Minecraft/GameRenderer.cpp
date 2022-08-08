#include "mcpch.h"
#include "Minecraft/GameRenderer.h"

#include "Minecraft/World/Chunk.h"


namespace Minecraft {

	struct RendererData
	{
		Ref<Font> HUDFont;

		Ref<Shader> BlockShader;
		Ref<Shader> WaterShader;

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

		s_Data->HUDFont = Font::Create("Content/Fonts/RobotoMono-Regular.ttf");

		s_Data->BlockShader = Shader::Create("Content/Shaders/Minecraft/Chunk.glsl");
		s_Data->WaterShader = Shader::Create("Content/Shaders/Minecraft/Water.glsl");

		TextureProperties props;
		props.SamplerFilter = TextureFilter::Nearest;
		props.SamplerWrap = TextureWrap::ClampToEdge;
		s_Data->BlockTextureAtlas = Texture2D::Create("Content/Textures/Atlases/terrain-atlas.png", props);

		s_Data->CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 1);
		s_Data->ChunkPositionUniformBuffer = UniformBuffer::Create(sizeof(RendererData::ChunkPositionData), 2);

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

		// -- Render solid --
		for (auto& chunkPosition : *world)
		{
			if (world->HasChunk(chunkPosition))
			{
				Ref<Chunk> chunk = world->GetChunk(chunkPosition);
				if (chunk->HasSolidBlocks())
				{
					s_Data->ChunkPositionBuffer.X = chunkPosition.x;
					s_Data->ChunkPositionBuffer.Y = chunkPosition.y;
					s_Data->ChunkPositionUniformBuffer->SetData(&s_Data->ChunkPositionBuffer, sizeof(RendererData::ChunkPositionData));

					RenderCommand::DrawIndexed(world->GetChunk(chunkPosition)->GetVertexArraySolid());
				}
			}
		}

		s_Data->WaterShader->Bind();

		// -- Render transparent --
		for (auto& chunkPosition : *world)
		{
			if (world->HasChunk(chunkPosition))
			{
				Ref<Chunk> chunk = world->GetChunk(chunkPosition);
				if (chunk->HasTransparentBlocks())
				{
					s_Data->ChunkPositionBuffer.X = chunkPosition.x;
					s_Data->ChunkPositionBuffer.Y = chunkPosition.y;
					s_Data->ChunkPositionUniformBuffer->SetData(&s_Data->ChunkPositionBuffer, sizeof(RendererData::ChunkPositionData));

					RenderCommand::DrawIndexed(world->GetChunk(chunkPosition)->GetVertexArrayTransparent());
				}
			}
		}
	}

	void GameRenderer::RenderHUD(const Ref<RenderCamera>& renderCamera)
	{
		RenderCommand::SetDepthTest(false);

		Renderer2D::BeginScene(renderCamera);

		//Renderer2D::DrawText("Hello World\nThis is now on a new line! Ain't that pog\nDesigned by Almaas!", s_Data->HUDFont, { 0.0f, 0.0f });

		// Crosshair
		Renderer2D::DrawSprite({ 0, 0 }, { 0.02f, 0.18f }, Color(0.8f, 0.95f));
		Renderer2D::DrawSprite({ 0, 0 }, { 0.18f, 0.02f }, Color(0.8f, 0.95f));

		Renderer2D::EndScene();
	}

}
