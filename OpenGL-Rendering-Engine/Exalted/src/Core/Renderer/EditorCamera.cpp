/***************************************************************************
 * Filename		: EditorCamera.cpp
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: Contains an implementation for an FPS camera used in the
 *                editor.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#include "expch.h"
#include "EditorCamera.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/MouseButtonCodes.h"
#include "Core/Events/ApplicationEvent.h"
#include "Core/Events/MouseEvent.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "UniformBuffer.h"
#include "glm/gtc/type_ptr.hpp"

namespace Exalted
{
	void EditorCamera::UpdateCamera(Timestep deltaTime)
	{
		if (Input::IsKeyPressed(EX_KEY_W))
			ProcessKeyboardEvent(CameraMovement::FORWARD, deltaTime);
		if (Input::IsKeyPressed(EX_KEY_A))
			ProcessKeyboardEvent(CameraMovement::LEFT, deltaTime);
		if (Input::IsKeyPressed(EX_KEY_S))
			ProcessKeyboardEvent(CameraMovement::BACKWARD, deltaTime);
		if (Input::IsKeyPressed(EX_KEY_D))
			ProcessKeyboardEvent(CameraMovement::RIGHT, deltaTime);
		if (Input::IsKeyPressed(EX_KEY_Q))
			ProcessKeyboardEvent(CameraMovement::UP, deltaTime);
		if (Input::IsKeyPressed(EX_KEY_E))
			ProcessKeyboardEvent(CameraMovement::DOWN, deltaTime);

		UpdateUBO();
	}

	void EditorCamera::UpdateTrack(Timestep deltaTime) 
	{
		EX_CORE_ASSERT(m_CurrentTrack, " Attempting to update the camera track with no currently set track! Set a track first!", true);
		m_CurrentTrack->Update(deltaTime, m_Position, m_Yaw, m_Pitch);
		UpdateCameraVectors();
		RecalculateViewMatrix();
		UpdateUBO(); // update ubo with camera data
	}

	void EditorCamera::SetTrack(const int index)
	{
		EX_CORE_ASSERT(index < m_CameraTracks.size(), " Attempting to set camera track to invalid index! {0}", 0, true);
		m_CurrentTrack = m_CameraTracks[index];
		m_CurrentTrack->PrepareTrack();
		
		//todo: maybe make this configurable ||  reset fov to initial value 
		m_FOV = 45.f; 
	}

	//todo: uncomment the flags
	void EditorCamera::OnImGuiRender() //todo: Give each camera a unique id (as in game component)
	{
		ImGui::Begin("Camera Transform");
		//ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		//ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.6f);
		ImGui::InputFloat3("Position", (float*)& m_Position);
		ImGui::InputFloat("Yaw", &m_Yaw);
		ImGui::InputFloat("Pitch", &m_Pitch);
		//ImGui::PopItemFlag();
		//ImGui::PopStyleVar();
		ImGui::InputFloat("Movement Speed", &m_MovementSpeed, 0.01f, 10.f);
		ImGui::InputFloat("Mouse Sensitivity", &m_MouseSensitivity, 0.01f, 10.f);
		ImGui::End();
	}

	void EditorCamera::UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
		front.y = sin(glm::radians(m_Pitch));
		front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
		m_Front = glm::normalize(front);
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}

	void EditorCamera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);;
		RecalculateViewProjectionMatrix();
	}

	void EditorCamera::OnEvent(Exalted::Event& event)
	{
		//if (event.GetEventType() == Exalted::EventType::WindowResize)
		//{
		//	const auto resizeEvent = dynamic_cast<Exalted::WindowResizeEvent&>(event);
		//	const auto windowWidth = resizeEvent.GetWidth();
		//	const auto windowHeight = resizeEvent.GetHeight();
		//	OnWindowResize(windowWidth, windowHeight);
		//}
		if ((event.GetEventType() == Exalted::EventType::MouseButtonPressed) && !m_MouseMoving)
		{
			auto& e = dynamic_cast<Exalted::MouseButtonPressedEvent&>(event);
			if (e.GetMouseButton() == EX_MOUSE_BUTTON_2)
			{
				m_FirstMouseMovement = true;
				m_ProcessingMouseMovement = true;
				m_MouseMoving = true;
			}
		}
		if (event.GetEventType() == Exalted::EventType::MouseButtonReleased)
		{
			auto& e = dynamic_cast<Exalted::MouseButtonReleasedEvent&>(event);
			if (e.GetMouseButton() == EX_MOUSE_BUTTON_2)
			{
				m_ProcessingMouseMovement = false;
				m_MouseMoving = false;
			}
		}
		if (event.GetEventType() == Exalted::EventType::MouseScrolled)
		{
			auto& e = dynamic_cast<Exalted::MouseScrolledEvent&>(event);
			ProcessMouseScrollEvent(e.GetYOffset());
		}
		if (m_ProcessingMouseMovement && (event.GetEventType() == Exalted::EventType::MouseMoved))
		{
			auto& e = dynamic_cast<Exalted::MouseMovedEvent&>(event);
			if (m_FirstMouseMovement)
			{
				m_LastMouseX = e.GetX();
				m_LastMouseY = e.GetY();
				m_FirstMouseMovement = false;
			}
			float xOffset = e.GetX() - m_LastMouseX;
			float yOffset = m_LastMouseY - e.GetY();

			m_LastMouseX = e.GetX();
			m_LastMouseY = e.GetY();
			ProcessRotationEvent(xOffset, yOffset);
		}
	}

	void EditorCamera::UpdateUBO(Ref<UniformBuffer>& ubo) const //todo: exists only for backwards compatibility with previous example scenes
	{
		ubo->Bind();
		Bytes offset = 0;
		Bytes size = sizeof(glm::mat4);
		Bytes sizeofVec4 = sizeof(glm::vec4);
		ubo->SetBufferSubData(offset, size, glm::value_ptr(GetViewMatrix()));
		offset += sizeof(glm::mat4);
		ubo->SetBufferSubData(offset, size, glm::value_ptr(glm::mat4(glm::mat3(GetViewMatrix()))));
		offset += sizeof(glm::mat4);
		ubo->SetBufferSubData(offset, size, glm::value_ptr(GetProjectionMatrix()));
		offset += sizeof(glm::mat4);
		ubo->SetBufferSubData(offset, size, glm::value_ptr(GetViewProjectionMatrix()));
		offset += sizeof(glm::mat4);
		ubo->SetBufferSubData(offset, sizeofVec4, glm::value_ptr(GetPosition()));
		offset += sizeof(glm::vec4);
		ubo->Unbind();
	}

	void EditorCamera::UpdateUBO() const
	{
		m_CameraUniformBuffer->Bind();
		Bytes offset = 0;
		Bytes size = sizeof(glm::mat4);
		Bytes sizeofVec4 = sizeof(glm::vec4);
		m_CameraUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(GetViewMatrix()));
		offset += sizeof(glm::mat4);
		m_CameraUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(glm::mat4(glm::mat3(GetViewMatrix()))));
		offset += sizeof(glm::mat4);
		m_CameraUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(GetProjectionMatrix()));
		offset += sizeof(glm::mat4);
		m_CameraUniformBuffer->SetBufferSubData(offset, size, glm::value_ptr(GetViewProjectionMatrix()));
		offset += sizeof(glm::mat4);
		m_CameraUniformBuffer->SetBufferSubData(offset, sizeofVec4, glm::value_ptr(GetPosition()));
		offset += sizeof(glm::vec4);
		m_CameraUniformBuffer->Unbind();
	}
	

	void EditorCamera::ProcessRotationEvent(float xOffset, float yOffset)
	{
		xOffset *= m_MouseSensitivity;
		yOffset *= m_MouseSensitivity;
		m_Yaw = glm::mod(m_Yaw + xOffset, 360.0f);
		m_Pitch += yOffset;

		if (m_Pitch > 89.f)
			m_Pitch = 89.f;
		else if (m_Pitch < -89.f)
			m_Pitch = -89.f;

		UpdateCameraVectors();
		RecalculateViewMatrix();
	}
	void EditorCamera::ProcessMouseScrollEvent(float yOffset)
	{
		if (m_FOV >= 1.0f && m_FOV <= 90.0f)
			m_FOV -= yOffset;
		if (m_FOV <= 1.0f)
			m_FOV = 1.0f;
		if (m_FOV >= 90.0f)
			m_FOV = 90.0f;
		RecalculateProjectionMatrix();
	}
	void EditorCamera::ProcessKeyboardEvent(CameraMovement direction, const float deltaTime)
	{
		const float velocity = m_MovementSpeed * deltaTime;
		if (direction == CameraMovement::FORWARD)
			m_Position += m_Front * velocity;
		if (direction == CameraMovement::BACKWARD)
			m_Position -= m_Front * velocity;
		if (direction == CameraMovement::LEFT)
			m_Position -= m_Right * velocity;
		if (direction == CameraMovement::RIGHT)
			m_Position += m_Right * velocity;
		if (direction == CameraMovement::UP)
			m_Position += m_WorldUp * velocity;
		if (direction == CameraMovement::DOWN)
			m_Position -= m_WorldUp * velocity;
		RecalculateViewMatrix();
	}
}
