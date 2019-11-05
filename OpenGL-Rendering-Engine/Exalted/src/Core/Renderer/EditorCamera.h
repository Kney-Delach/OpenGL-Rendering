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
#include "Core/Events/Event.h"


namespace Exalted
{
	enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };
	
	class EditorCamera : public PerspectiveCamera
	{
	public:
		EditorCamera(float horizontalFOV = 45.f, float aspectRatio = 1280.f/720.f, float zNear = 0.1f, float zFar = 100.f)
			: PerspectiveCamera(horizontalFOV,aspectRatio, zNear, zFar)
		{
			UpdateCameraVectors();
			RecalculateViewMatrix();
		}
		void UpdateCamera(Timestep deltaTime);
		void OnImGuiRender();
		void OnEvent(Exalted::Event& event);
	private:
		inline void RecalculateViewProjectionMatrix() override { m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; }
		void UpdateCameraVectors();
		void RecalculateViewMatrix() override;
		void ProcessRotationEvent(float xOffset, float yOffset);
		void ProcessMouseScrollEvent(float yOffset);
		void ProcessKeyboardEvent(CameraMovement direction, const float deltaTime);
	private:
		glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		float m_Yaw = 45.f;
		float m_Pitch = 0.f;
		float m_MovementSpeed = 2.5f;
		float m_MouseSensitivity = 0.1f;
	private: // Mouse Event Handling Variables
		bool m_ProcessingMouseMovement = false;
		bool m_MouseMoving = false;
		bool m_FirstMouseMovement = true;
		float m_LastMouseX, m_LastMouseY;
	};
}