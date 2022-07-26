#pragma once

using namespace Moon;

#include "Minecraft/GameCamera.h"
#include "Minecraft/World.h"


namespace Minecraft {

	class GameLayer : public Layer
	{
	public:
		GameLayer();
		virtual ~GameLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		Ref<World> m_World = nullptr;
		Ref<GameCamera> m_Camera = nullptr;

		Ref<OrthographicCamera> m_CameraHUD = nullptr;

		float m_FrameTime = 0.0f;
		float m_LastTickTime = 0.0f;
	};

}
