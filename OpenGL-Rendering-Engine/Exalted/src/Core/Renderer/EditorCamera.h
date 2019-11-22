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
#include "UniformBuffer.h"
#include "CameraTrack.h"


namespace Exalted
{
	class UniformBuffer;

	enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };
	
	class EditorCamera : public PerspectiveCamera
	{
	public:
		EditorCamera(float horizontalFOV = 45.f, float aspectRatio = 1280.f/720.f, float zNear = 0.1f, float zFar = 100.f, Bytes bufferIndex = 2)
			: PerspectiveCamera(horizontalFOV,aspectRatio, zNear, zFar)
		{
			// setup ubo
			Bytes bufferSize = 4 * sizeof(glm::mat4) + sizeof(glm::vec4);;
			m_CameraUniformBuffer = UniformBuffer::Create(bufferSize);
			const Bytes offset = 0;
			m_CameraUniformBuffer->BindBufferRange(bufferIndex, offset, bufferSize);
			
			UpdateCameraVectors();
			RecalculateViewMatrix();
		}

		void UpdateCamera(Timestep deltaTime); // manual camera updates

		// have a list of tracks
		// track functions
		void AddTrack(const Ref<CameraTrack>& cameraTrack) { m_CameraTracks.emplace_back(cameraTrack); }
		void UpdateTrack(Timestep deltaTime);  // automatic track camera updates
		void SetTrack(const int index);
		void ResetMovementVariables()
		{
			m_ProcessingMouseMovement = false;
			m_MouseMoving = false;
		}
		
		void OnImGuiRender();
		void OnEvent(Event& event);
		void UpdateUBO(Ref<UniformBuffer>& ubo) const;
		void UpdateUBO() const;
		void SetMouseSensitivity(const float sensitivity) { m_MouseSensitivity = sensitivity; }
		void SetMouseSpeed(const float speed) { m_MovementSpeed = speed; }
		float GetFOV() { return m_FOV; } //todo: move this to parent class
		float GetYaw() { return m_Yaw; }
		float GetPitch() { return m_Pitch; }
		inline glm::vec3& GetFront() { return m_Front; }
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
	private:
		Ref<CameraTrack> m_CurrentTrack; 
		std::vector<Ref<CameraTrack>> m_CameraTracks;
		Ref<UniformBuffer> m_CameraUniformBuffer; //todo: verify this works correctly.
	private:
		//todo: insert camera track variables 
	private: // Mouse Event Handling Variables
		bool m_ProcessingMouseMovement = false;
		bool m_MouseMoving = false;
		bool m_FirstMouseMovement = true;
		float m_LastMouseX, m_LastMouseY;
	};
}
