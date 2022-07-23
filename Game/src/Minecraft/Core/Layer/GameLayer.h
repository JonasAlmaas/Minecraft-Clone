#pragma once

using namespace Moon;

#include "Minecraft/Camera/GameCamera.h"
#include "Minecraft/World/World.h"


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

	private:
		Ref<World> m_World = nullptr;
		Ref<GameCamera> m_Camera = nullptr;

		float m_FrameTime = 0;

	};

}
