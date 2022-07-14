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
		m_World = CreateRef<World>(seed, &m_Camera->GetPosition());
	}

	void GameLayer::OnUpdate(Timestep ts)
	{
		m_FrameTime = ts;

		// ---- Tick ----
		float timeNow = Time::Get();
		Timestep tickTimestep = timeNow - m_LastTickTime;
		if (tickTimestep >= 1.0f / 20.0f)
		{
			m_World->Tick(tickTimestep);
			m_LastTickTime = timeNow;
		}

		// ---- Update ----

		m_Camera->OnUpdate(ts);

		// ---- Render ----

		RenderCommand::SetClearColor({ 0.431f, 0.8f, 1.0f, 1.0f });
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
