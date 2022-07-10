#pragma once

using namespace Moon;


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

	};

}
