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

// far, near, right, left, top, bottom
namespace Exalted 
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
		RecalculateViewMatrix();
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: OrthographicCamera(left, right, bottom, top, -1.0f, 100000.f)
	{
	}

	PerspectiveCamera::PerspectiveCamera(float horizontalFov, float aspectRatio, float zNear, float zFar)
		: m_FOV(horizontalFov), m_AspectRatio(aspectRatio), m_zNear(zNear), m_zFar(zFar)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(horizontalFov), aspectRatio, zNear, zFar);
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetFOV(float horizontalFOV)
	{
		if (m_FOV == horizontalFOV) return;
		m_FOV = horizontalFOV;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_zNear, m_zFar);
		RecalculateViewMatrix();
	}

	void Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		//m_ViewMatrix = glm::inverse(transform); //todo: Put this back for cleaner implementations.
		m_ViewProjectionMatrix = m_ProjectionMatrix * transform; // * transform;
	}
}