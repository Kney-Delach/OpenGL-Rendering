#include "expch.h"
#include "Core/Application.h"
#include "GameComponent.h"

#include "GLFW/glfw3.h" //todo: abstract this from here

namespace Exalted
{
	void ScaleGrowComponent::Update(Timestep deltaTime)
	{
		if (glfwGetTime() > m_StartTime)
		{
			if (!(m_Transform->Scale.x >= m_MaxScale.x && m_Transform->Scale.y >= m_MaxScale.y && m_Transform->Scale.z >= m_MaxScale.z))
				m_Transform->Scale = m_Transform->Scale + m_ScaleRate * deltaTime.GetSeconds(), m_MaxScale;
		}
	}
}