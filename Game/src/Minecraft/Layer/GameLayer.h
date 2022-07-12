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
		virtual void OnDetach() override {};
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		Ref<World> m_World = nullptr;
		Ref<GameCamera> m_Camera;

		float m_FrameTime = 0;

	};

}
