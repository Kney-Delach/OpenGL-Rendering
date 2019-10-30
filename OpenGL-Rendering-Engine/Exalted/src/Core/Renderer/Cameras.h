/***************************************************************************
 * Filename		: Cameras.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains the camera declarations for this engine.
 *                Supported camera types: Orthographic
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

//todo: Replace glm with Mathex math library.

namespace Exalted 
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);	// Constructor with assumed near and far clip planes of -1 and 1
		OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar);

		_NODISCARD inline const glm::vec3& GetPosition() const { return m_Position; }
		inline void SetPosition(const glm::vec3& position)
		{
			m_Position = position; 
			RecalculateViewMatrix();
		}

		_NODISCARD inline float GetRotation() const { return m_Rotation; }
		inline void SetRotation(float rotation)
		{
			m_Rotation = rotation; 
			RecalculateViewMatrix();
		}

		_NODISCARD const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		_NODISCARD const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		_NODISCARD const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;		// The projection matrix for the camera.
		glm::mat4 m_ViewMatrix;				// Inverse of transformation matrix of camera
		glm::mat4 m_ViewProjectionMatrix;	// The view projection matrix for the camera. This should usually be passed into the shader.
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };	// The current position of the camera.
		float m_Rotation = 0.0f;	// The rotation of the camera along the Z-axis, only exists for orthographic cameras.
	};
}