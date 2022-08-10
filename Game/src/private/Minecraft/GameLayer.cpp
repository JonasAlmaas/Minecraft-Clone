#include "mcpch.h"
#include "Minecraft/GameLayer.h"

#include "Minecraft/Math/Ray.h"
#include "Minecraft/GameRenderer.h"
#include "Minecraft/World/WorldGen.h"


namespace Minecraft {

	GameLayer::GameLayer()
		: Layer("GameLayer")
	{
	}

	void GameLayer::OnAttach()
	{
		GameRenderer::Init();

		Application::Get().GetWindow().DisableCursor();

		m_Camera = CreateRef<GameCamera>();
		m_Camera->SetPosition({ 0, 0, 64 });

		m_CameraHUD = CreateRef<OrthographicCamera>(10.0f);

		float windowWidth = (float)Application::Get().GetWindow().GetWidth();
		float windowHeight = (float)Application::Get().GetWindow().GetHeight();
		m_Camera->SetViewportSize(windowWidth, windowHeight);
		m_CameraHUD->SetViewportSize(windowWidth, windowHeight);

		int seed = Random::UInt32();
		WorldGen::Init(seed);

		m_World = CreateRef<World>(&m_Camera->GetPosition());
	}

	void GameLayer::OnDetach()
	{
		WorldGen::Shutdown();
		GameRenderer::Shutdown();
	}

	void GameLayer::OnUpdate(Timestep ts)
	{
		m_FrameTime = ts;

		m_World->OnUpdate(ts);

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

		GameRenderer::RenderWorld(m_World, m_Camera->GetViewProjection());
		GameRenderer::RenderHUD(m_CameraHUD);
	}

	void GameLayer::OnImGuiRender()
	{
		ImGui::Begin("Statistics");
		ImGui::Text("Frame Time: %.2fms", m_FrameTime * 1000.0f);
		ImGui::Text("Fps: %d", (int)(1.0f / m_FrameTime));

		ImGui::Separator();

		glm::vec3 playerPos = m_Camera->GetPosition();
		ImGui::Text("Position: x: %.2f  -  y: %.2f  -  z: %.2f", playerPos.x, playerPos.y, playerPos.z);

		ImGui::End();
	}

	// -- Event Handling --
	void GameLayer::OnEvent(Event & e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(ME_BIND_EVENT_FN(GameLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<MouseButtonPressedEvent>(ME_BIND_EVENT_FN(GameLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(ME_BIND_EVENT_FN(GameLayer::OnWindowResizeEvent));

		m_Camera->OnEvent(e);
	}

	bool GameLayer::OnKeyPressedEvent(KeyPressedEvent & e)
	{
		if (e.IsRepeat())
			return false;

		switch (e.GetKeyCode())
		{
			case Key::F5:
			{
				GameRenderer::ReloadShaders();
			}
		}

		return false;
	}

	bool GameLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::Button0)
		{
			Ray ray(m_Camera->GetPosition(), m_Camera->GetForwardVector(), 5.0f);

			RayWorldHitResult hitResult;
			if (ray.WorldIntersection(m_World, hitResult))
			{
				WorldBlockPosition blockPos = { hitResult.HitBlock.x, hitResult.HitBlock.y, hitResult.HitBlock.z };
				if (m_World->HasBlock(blockPos))
					m_World->BreakBlock(blockPos);
			}
		}

		return false;
	}

	bool GameLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		m_CameraHUD->SetViewportSize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}
