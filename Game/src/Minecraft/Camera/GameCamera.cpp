#include "mcpch.h"
#include "Minecraft/Camera/GameCamera.h"


namespace Moon {

	GameCamera::GameCamera()
	{
		m_Pitch = glm::radians(-90.0f);
	}

	GameCamera::GameCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: PerspectiveCamera(fov, aspectRatio, nearClip, farClip)
	{
		m_Pitch = glm::radians(-90.0f);

		m_LastMouseX = Input::GetMouseX();
		m_LastMouseY = Input::GetMouseY();
	}

	void GameCamera::OnUpdate(Timestep ts)
	{
		// Move Forward
		if (Input::IsKeyPressed(Key::W))
		{
			m_Position.x += sin(m_Yaw) * m_MoveSpeed * ts;
			m_Position.y += cos(m_Yaw) * m_MoveSpeed * ts;
		}

		// Move Back
		if (Input::IsKeyPressed(Key::S))
		{
			m_Position.x -= sin(m_Yaw) * m_MoveSpeed * ts;
			m_Position.y -= cos(m_Yaw) * m_MoveSpeed * ts;
		}

		// Move Left
		if (Input::IsKeyPressed(Key::A))
		{
			m_Position.x -= cos(m_Yaw) * m_MoveSpeed * ts;
			m_Position.y += sin(m_Yaw) * m_MoveSpeed * ts;
		}

		// Move Right
		if (Input::IsKeyPressed(Key::D))
		{
			m_Position.x += cos(m_Yaw) * m_MoveSpeed * ts;
			m_Position.y -= sin(m_Yaw) * m_MoveSpeed * ts;
		}

		// Move Up
		if (Input::IsKeyPressed(Key::Space))
		{
			m_Position.z += m_MoveSpeed * ts;
		}

		// Move Down
		if (Input::IsKeyPressed(Key::LeftShift))
		{
			m_Position.z -= m_MoveSpeed * ts;
		}

		UpdateView();
	}

	// -- Event Handling --

	void GameCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(ME_BIND_EVENT_FN(GameCamera::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(ME_BIND_EVENT_FN(GameCamera::OnKeyReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(ME_BIND_EVENT_FN(GameCamera::OnMouseMovedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(ME_BIND_EVENT_FN(GameCamera::OnWindowResizeEvent));
	}

	bool GameCamera::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;

		if (e.GetKeyCode() == Key::LeftControl)
			m_MoveSpeed = m_MoveSpeedFast;

		return false;
	}

	bool GameCamera::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		if (e.GetKeyCode() == Key::LeftControl)
			m_MoveSpeed = m_MoveSpeedNormal;

		return false;
	}

	bool GameCamera::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		float offsetX = e.GetX() - m_LastMouseX;
		float offsetY = e.GetY() - m_LastMouseY;

		m_LastMouseX = e.GetX();
		m_LastMouseY = e.GetY();

		m_Pitch += m_Sensitivity * offsetY;
		m_Pitch = glm::clamp<float>(m_Pitch, -glm::radians(180.0f), 0.0f);

		m_Yaw += m_Sensitivity * offsetX;

		return false;
	}

	bool GameCamera::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		SetViewportSize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}
