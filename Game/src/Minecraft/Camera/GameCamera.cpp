#include "mcpch.h"
#include "Minecraft/Camera/GameCamera.h"


namespace Moon {

	GameCamera::GameCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		m_LastMouseX = Input::GetMouseX();
		m_LastMouseY = Input::GetMouseY();

		UpdateView();
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

	void GameCamera::SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateProjection();
	}

	glm::vec3 GameCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 GameCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 GameCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat GameCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, 0.0f, -m_Yaw));
	}

	void GameCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void GameCamera::UpdateView()
	{
		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	// -- Event Handling --

	void GameCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(ME_BIND_EVENT_FN(GameCamera::OnMouseMovedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(ME_BIND_EVENT_FN(GameCamera::OnWindowResizeEvent));
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
		m_ViewportWidth = e.GetWidth();
		m_ViewportHeight = e.GetHeight();
		return false;
	}

}
