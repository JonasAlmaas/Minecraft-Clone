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
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		s_Data.BlockShader = Shader::Create("Content/Shaders/Block.glsl");

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);
	}

	void Renderer::RenderWorld(const Ref<World>& world, const glm::mat4& viewProjectionMatrix)
	{
		s_Data.CameraBuffer.ViewProjection = viewProjectionMatrix;
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));

		s_Data.BlockShader->Bind();
		RenderCommand::DrawIndexed(world->m_VA);
	}

}
