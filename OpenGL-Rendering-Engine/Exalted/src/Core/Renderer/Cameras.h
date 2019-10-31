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
	class Camera
	{
	public:
		Camera() = default;
		virtual ~Camera() = default;
		_NODISCARD inline const glm::vec3& GetPosition() const { return m_Position; }
		inline void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
			RecalculateProjectionMatrix();
		}
		_NODISCARD const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		_NODISCARD const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		_NODISCARD virtual const glm::mat4& GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }
	protected:
		virtual void RecalculateProjectionMatrix() = 0;
	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix; // pass this to the shader
		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);	
		float m_zNear;
		float m_zFar;
	};

	class OrthographicCamera : public Camera 
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar);
		virtual ~OrthographicCamera() = default;
		virtual void RecalculateProjectionMatrix() override;
		_NODISCARD inline float GetRotation() const { return m_Rotation; }
		inline void SetRotation(float rotation)
		{
			m_Rotation = rotation;
			RecalculateProjectionMatrix();
		}
	protected:
		float m_Rotation = 0.0f;	// The rotation of the camera along the Z-axis
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float horizontalFov, float aspectRatio, float zNear = 1.0f, float zFar = 10000.f);
		virtual ~PerspectiveCamera() = default;
		void SetFOV(float horizontalFOV);
		virtual void RecalculateProjectionMatrix() override;
	protected:
		float m_FOV;
		float m_AspectRatio;
	};
}