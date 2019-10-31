/***************************************************************************
 * Filename		: EditorCamera.h
 * Name			: Ori Lazar
 * Date			: 31/10/2019
 * Description	: Contains an implementation for an FPS camera used in the 
 *                editor for this engine.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cameras.h"
#include "Core/Core/Timestep.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"

//todo:, I created a custom lookAt function in "BuildViewMatrix",to  allow myself to abstract it to the Mathex math library at a later date. 

namespace Exalted
{
	enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };
	
	class EditorCamera : public PerspectiveCamera
	{
	public:
		EditorCamera(float horizontalFOV = 45.f, float aspectRatio = 1280.f/720.f, float zNear = 0.1f, float zFar = 100.f)
			: PerspectiveCamera(horizontalFOV,aspectRatio, zNear, zFar)
		{
			UpdateCameraVectors();
			RecalculateViewMatrix();
		}

		inline void UpdateCamera(Timestep deltaTime)
		{
			if (Input::IsKeyPressed(EX_KEY_W))
				ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
			if (Input::IsKeyPressed(EX_KEY_A))
				ProcessKeyboard(CameraMovement::LEFT, deltaTime);
			if (Input::IsKeyPressed(EX_KEY_S))
				ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
			if (Input::IsKeyPressed(EX_KEY_D))
				ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
		}

		inline void ProcessRotationEvent(float xOffset, float yOffset)
		{
			xOffset *= m_MouseSensitivity;
			yOffset *= m_MouseSensitivity;
			m_Yaw += xOffset;
			m_Pitch += yOffset;
			glm::clamp(m_Pitch, -89.0f, 89.0f);

			UpdateCameraVectors();
			RecalculateViewMatrix();
		}

		inline void ProcessMouseScrollEvent(float yOffset)
		{
			if (m_FOV >= 1.0f && m_FOV <= 90.0f)
				m_FOV -= yOffset;
			if (m_FOV <= 1.0f)
				m_FOV = 1.0f;
			if (m_FOV >= 90.0f)
				m_FOV = 90.0f;
			RecalculateProjectionMatrix();
		}

	private:
		inline void UpdateCameraVectors()
		{
			glm::vec3 front;
			front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			front.y = sin(glm::radians(m_Pitch));
			front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			m_Front = glm::normalize(front);
			m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
			m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		}
		inline void ProcessKeyboard(CameraMovement direction, const float deltaTime)
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
			RecalculateViewMatrix();
		}
		_NODISCARD inline glm::mat4 BuildViewMatrix() const
		{
			glm::mat4 translation = glm::mat4(1.0f);
			translation[3][0] = -m_Position.x;
			translation[3][1] = -m_Position.y;
			translation[3][2] = -m_Position.z;

			const glm::vec3 cameraDirectionZ = glm::normalize(m_Position - (m_Position + m_Front));
			const glm::vec3 cameraDirectionX = glm::normalize(glm::cross(glm::normalize(m_Up), cameraDirectionZ));
			const glm::vec3 cameraDirectionY = glm::cross(cameraDirectionZ, cameraDirectionX);

			glm::mat4 rotation = glm::mat4(1.0f);
			rotation[0][0] = cameraDirectionX.x;
			rotation[1][0] = cameraDirectionX.y;
			rotation[2][0] = cameraDirectionX.z;
			rotation[0][1] = cameraDirectionY.x;
			rotation[1][1] = cameraDirectionY.y;
			rotation[2][1] = cameraDirectionY.z;
			rotation[0][2] = cameraDirectionZ.x;
			rotation[1][2] = cameraDirectionZ.y;
			rotation[2][2] = cameraDirectionZ.z;

			return rotation * translation;
		}

		inline void RecalculateViewMatrix() override
		{
			m_ViewMatrix = BuildViewMatrix();
			RecalculateViewProjectionMatrix();
		}

		inline void RecalculateViewProjectionMatrix() override { m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; }
	private:
		glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		float m_Yaw = 90.f;
		float m_Pitch = 0.f;
		float m_MovementSpeed = 2.5f;
		float m_MouseSensitivity = 0.1f;
	};
}