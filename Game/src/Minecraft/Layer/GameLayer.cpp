#include "mcpch.h"
#include "Minecraft/Layer/GameLayer.h"

#include "Minecraft/Renderer/Renderer.h"

namespace Minecraft {

	GameLayer::GameLayer()
		: Layer("GameLayer")
	{
	}

	void GameLayer::OnAttach()
	{
		Renderer::Init();

		Application::Get().GetWindow().DisableCursor();

		m_Camera = CreateRef<GameCamera>();

		float windowWidth = Application::Get().GetWindow().GetWidth();
		float windowHeight = Application::Get().GetWindow().GetHeight();
		m_Camera->SetViewportSize(windowWidth, windowHeight);

		uint64_t seed = Random::UInt();
		m_World = CreateRef<World>(seed);
	}

	void GameLayer::OnUpdate(Timestep ts)
	{
		m_FrameTime = ts;

		// ---- Update ----

		m_Camera->OnUpdate(ts);

		// ---- Render ----

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear();

		Renderer::RenderWorld(m_World, m_Camera->GetViewProjection());
	}

	void GameLayer::OnImGuiRender()
	{
		ImGui::Begin("Statistics");

		ImGui::Text("Frame Time: %.2fms", m_FrameTime * 1000.0f);
		ImGui::Text("Fps: %d", (int)(1.0f / m_FrameTime));

		ImGui::End();
	}

	// -- Event Handling --

	void GameLayer::OnEvent(Event& e)
	{
		m_Camera->OnEvent(e);
	}

}
