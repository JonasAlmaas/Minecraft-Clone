#pragma once

using namespace Moon;

#include <Moon/Core/Renderer/Camera/Camera.h>


namespace Moon {

	class GameCamera : public Camera
	{
	public:
		GameCamera() = default;
		GameCamera(float fov, float aspectRatio, float nearClip, float farClip);

		~GameCamera() = default;

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void SetViewportSize(float width, float height);

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		glm::quat GetOrientation() const;
		const glm::vec3& GetPosition() const { return m_Position; }

		float GetPitch() const { return glm::degrees(m_Pitch); }
		float GetYaw() const { return glm::degrees(m_Yaw); }
		float GetRoll() const { return glm::degrees(m_Roll); }

	private:
		void UpdateProjection();
		void UpdateView();

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

		float m_FOV = 45.0f;
		float m_AspectRatio = 1.778f;
		float m_NearClip = 0.1f;
		float m_FarClip = 10000.0f;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 5.0f };

		float m_Pitch = glm::radians(-90.0f);
		float m_Yaw = 0.0f;
		float m_Roll = 0.0f;

		float m_ViewportWidth = 1920.0f;
		float m_ViewportHeight = 1080.0f;

	};

}
