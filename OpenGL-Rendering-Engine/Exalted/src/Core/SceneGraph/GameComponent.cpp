/***************************************************************************
 * Filename		: GameComponent.cpp
 * Name			: Ori Lazar
 * Date			: 03/11/2019
 * Description	: Contains the implementations for the game components.
				  If thinking about this in terms of trees, this is the node components.
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
#include "Core/Application.h"
#include "GameComponent.h"

#include "GLFW/glfw3.h" //todo: abstract this from here

namespace Exalted
{
	void TranslateAxisPositiveY::Update(Timestep deltaTime)
	{
		if(glfwGetTime() > m_StartTime)
		{
			if (!(m_Transform->Position.y >= m_GoalPosition))
				m_Transform->Position.y = m_Transform->Position.y + m_TranslationRate * deltaTime.GetSeconds();
		}
	}

	void ScaleGrowComponent::Update(Timestep deltaTime)
	{
		if (glfwGetTime() > m_StartTime)
		{
			if (!(m_Transform->Scale.x >= m_MaxScale.x && m_Transform->Scale.y >= m_MaxScale.y && m_Transform->Scale.z >= m_MaxScale.z))
			{
				m_Transform->Scale = m_Transform->Scale + m_ScaleRate * deltaTime.GetSeconds();
				if (m_Transform->Scale.x > m_MaxScale.x)
					m_Transform->Scale.x = m_MaxScale.x;
				if (m_Transform->Scale.y > m_MaxScale.y)
					m_Transform->Scale.y = m_MaxScale.y;
				if (m_Transform->Scale.z > m_MaxScale.z)
					m_Transform->Scale.z = m_MaxScale.z;
			}
		}
	}
}