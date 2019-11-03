/***************************************************************************
 * Filename		: Cameras.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains the camera implementations for this engine.
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
#include "expch.h"
#include "Cameras.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Exalted 
{
	// ---------------------------- Orthographic Camera ---------------------------- //

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
		RecalculateViewMatrix();
		RecalculateViewProjectionMatrix();
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: OrthographicCamera(left, right, bottom, top, -1.0f, 100000.f)
	{
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
	}

	void OrthographicCamera::RecalculateViewProjectionMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	// ---------------------------- Perspective Camera ---------------------------- //

	PerspectiveCamera::PerspectiveCamera(float horizontalFov, float aspectRatio, float zNear, float zFar)
		: m_FOV(horizontalFov), m_AspectRatio(aspectRatio)
	{
		m_zNear = zNear;
		m_zFar = zFar;
		RecalculateProjectionMatrix();
	}

	void PerspectiveCamera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_zNear, m_zFar);
		RecalculateViewProjectionMatrix();
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		RecalculateViewProjectionMatrix();
	}

	void PerspectiveCamera::RecalculateViewProjectionMatrix()
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetFOV(float horizontalFOV)
	{
		if (m_FOV == horizontalFOV) return;
		m_FOV = horizontalFOV;
		RecalculateProjectionMatrix();
		RecalculateViewProjectionMatrix();
	}

	void PerspectiveCamera::SetAspectRatio(const float aspectRatio)
	{
		if(aspectRatio > 0)
		{
			m_AspectRatio = aspectRatio;
			RecalculateProjectionMatrix();
		}
		else
			EX_ERROR("Attempting to insert invalid aspect ratio to perspective camera: -> Value: {0} ", 0);
	}

	void PerspectiveCamera::SetNearZ(const float zNear)
	{
		m_zNear = zNear;
		RecalculateProjectionMatrix();
	}

	void PerspectiveCamera::SetFarZ(const float zFar)
	{
		m_zFar = zFar;
		RecalculateProjectionMatrix();
	}
}