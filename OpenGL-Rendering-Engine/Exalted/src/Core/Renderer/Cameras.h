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
		virtual void OnWindowResize(unsigned int newWidth, unsigned int newHeight) {};
		inline void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
			RecalculateViewMatrix();
			RecalculateViewProjectionMatrix();
		}
		_NODISCARD inline const glm::vec3& GetPosition() const { return m_Position; }
		_NODISCARD const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		_NODISCARD const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		_NODISCARD const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	protected:
		virtual void RecalculateViewMatrix() = 0;
		virtual void RecalculateViewProjectionMatrix() = 0;
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
		_NODISCARD inline float GetRotation() const { return m_Rotation; }
		inline void SetRotation(float rotation)
		{
			m_Rotation = rotation;
			RecalculateViewMatrix();
			RecalculateViewProjectionMatrix();
		}
	protected:
		virtual void RecalculateViewMatrix() override;
		virtual void RecalculateViewProjectionMatrix() override;
	protected:
		float m_Rotation = 0.0f;
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(const float horizontalFov = 45.0f, const float aspectRatio = 1280.f/720.f, const float zNear = 1.0f, const float zFar = 10000.f);
		virtual ~PerspectiveCamera() = default;
		inline virtual void OnWindowResize(unsigned int windowWidth, unsigned int windowHeight) override
		{
			if (windowWidth == 0 || windowHeight == 0)
				return;
			SetAspectRatio(static_cast<float>(windowWidth) / static_cast<float>(windowHeight));
		};
		void SetFOV(const float horizontalFOV);
		void SetAspectRatio(const float aspectRatio);
		void SetNearZ(const float zNear);
		void SetFarZ(const float zFar);
	protected:
		void RecalculateProjectionMatrix();
		virtual void RecalculateViewMatrix() override;
		virtual void RecalculateViewProjectionMatrix() override;
	protected:
		float m_FOV;
		float m_AspectRatio;
	};
}