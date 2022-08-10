#include "mcpch.h"
#include "Minecraft/GameRenderer.h"

#include "Minecraft/World/Chunk.h"


namespace Minecraft {

	struct RendererData
	{
		//Ref<Font> HUDFont;

		Ref<Shader> TerrainShader;
		//Ref<Shader> WaterShader;

		Ref<Texture2D> TerrainTextureAtlas;

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

		//s_Data->HUDFont = Font::Create("Content/Fonts/RobotoMono-Regular.ttf");

		s_Data->TerrainShader = Shader::Create("Content/Shaders/Minecraft/Terrain.glsl");
		//s_Data->WaterShader = Shader::Create("Content/Shaders/Minecraft/Water.glsl");

		TextureProperties props;
		props.SamplerFilter = TextureFilter::Nearest;
		props.SamplerWrap = TextureWrap::ClampToEdge;
		s_Data->TerrainTextureAtlas = Texture2D::Create("Content/Textures/Atlases/terrainAtlas.png", props);

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
		s_Data->TerrainShader->Reload();
	}

	void GameRenderer::RenderWorld(const Ref<World>& world, const glm::mat4& viewProjectionMatrix)
	{
		RenderCommand::SetDepthTest(true);

		s_Data->CameraBuffer.ViewProjection = viewProjectionMatrix;
		s_Data->CameraUniformBuffer->SetData(&s_Data->CameraBuffer, sizeof(RendererData::CameraData));

		// -- Render Terrain --
		s_Data->TerrainTextureAtlas->Bind();
		s_Data->TerrainShader->Bind();

		for (auto it = --world->end(); it >= world->begin(); it--)
		{
			if (world->HasChunk(*it))
			{
				Ref<Chunk> chunk = world->GetChunk(*it);

				bool hasSolidTerrain = chunk->HasSolidTerrain();
				bool hasTransparentTerrain = chunk->HasTransparentTerrain();

				if (hasSolidTerrain || hasTransparentTerrain)
				{
					s_Data->ChunkPositionBuffer.X = it->x;
					s_Data->ChunkPositionBuffer.Y = it->y;
					s_Data->ChunkPositionUniformBuffer->SetData(&s_Data->ChunkPositionBuffer, sizeof(RendererData::ChunkPositionData));
				}

				if (hasSolidTerrain)
					RenderCommand::DrawIndexed(chunk->GetSolidTerrainMesh());

				if (hasTransparentTerrain)
					RenderCommand::DrawIndexed(chunk->GetTransparentTerrainMesh());
			}
		}

		// -- Render Water --
		//s_Data->WaterShader->Bind();
		/*
		for (auto it = --world->end(); it >= world->begin(); it--)
		{
			if (world->HasChunk(*it))
			{
				Ref<Chunk> chunk = world->GetChunk(*it);

				if (chunk->HasWater())
				{
					s_Data->ChunkPositionBuffer.X = it->x;
					s_Data->ChunkPositionBuffer.Y = it->y;
					s_Data->ChunkPositionUniformBuffer->SetData(&s_Data->ChunkPositionBuffer, sizeof(RendererData::ChunkPositionData));

					RenderCommand::DrawIndexed(world->GetChunk(chunkPosition)->GetWaterMesh());
				}
			}
		}
		*/
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
