#pragma once

using namespace Moon;


namespace Moon {

	class GameCamera : public PerspectiveCamera
	{
	public:
		GameCamera();
		GameCamera(float fov, float aspectRatio, float nearClip, float farClip);

		~GameCamera() = default;

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);

		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		float m_MoveSpeedNormal = 10.0f;
		float m_MoveSpeedFast = 100.0f;
		float m_MoveSpeed = m_MoveSpeedNormal;

		float m_Sensitivity = 0.001f;

		float m_LastMouseX = 0.0f;
		float m_LastMouseY = 0.0f;

	};

}
