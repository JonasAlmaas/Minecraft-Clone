#include "mcpch.h"
#include "Minecraft/GameLayer.h"

#include "Minecraft/Math/Ray.h"
#include "Minecraft/GameRenderer.h"


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
		m_CameraHUD = CreateRef<OrthographicCamera>(10.0f, 1.778f);

		float windowWidth = (float)Application::Get().GetWindow().GetWidth();
		float windowHeight = (float)Application::Get().GetWindow().GetHeight();
		m_Camera->SetViewportSize(windowWidth, windowHeight);
		m_CameraHUD->SetViewportSize(windowWidth, windowHeight);

		uint64_t seed = Random::UInt();
		m_World = CreateRef<World>(seed, &m_Camera->GetPosition());
	}

	void GameLayer::OnDetach()
	{
		GameRenderer::Shutdown();
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

		GameRenderer::RenderWorld(m_World, m_Camera->GetViewProjection());
		GameRenderer::RenderHUD(m_CameraHUD);
	}

	void GameLayer::OnImGuiRender()
	{
		ImGui::Begin("Statistics");
		ImGui::Text("Frame Time: %.2fms", m_FrameTime * 1000.0f);
		ImGui::Text("Fps: %d", (int)(1.0f / m_FrameTime));
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
				Chunk::Position hitChunkPosition = {
					(int)glm::floor(hitResult.HitBlock.x / 16.0f),
					(int)glm::floor(hitResult.HitBlock.y / 16.0f)
				};

				ChunkBlock::Position hitBlockPosition = {
					(int)glm::floor(hitResult.HitBlock.x) % 16,
					(int)glm::floor(hitResult.HitBlock.y) % 16,
					(int)glm::floor(hitResult.HitBlock.z)
				};

				if (hitBlockPosition.x < 0)
					hitBlockPosition.x = 16 + hitBlockPosition.x;

				if (hitBlockPosition.y < 0)
					hitBlockPosition.y = 16 + hitBlockPosition.y;

				m_World->GetChunk(hitChunkPosition)->BreakBlock(hitBlockPosition);
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
