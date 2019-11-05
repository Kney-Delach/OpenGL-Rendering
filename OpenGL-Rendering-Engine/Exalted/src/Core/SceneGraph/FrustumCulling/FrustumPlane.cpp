/***************************************************************************
 * Filename		: GameObject.h
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: Contains an implementation of a plane, used for frustum
 *                culling optimizations in scene graph rendering.
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
#include "FrustumPlane.h"

namespace Exalted
{
	FrustumPlane::FrustumPlane(const glm::vec3& normalVector, float distanceFromOrigin, bool normalize)
	{
		if(normalize)
		{
			float length = glm::sqrt(glm::dot(normalVector, normalVector)); //todo: Verify this works correctly 

			m_Normal = normalVector / length;
			m_Distance = distanceFromOrigin / length; 
		}
		else
		{
			m_Normal = normalVector;
			m_Distance = distanceFromOrigin;
		}
	}

	// plane equation, with added allowance for the radius of the bounding sphere. 
	bool FrustumPlane::SphereInPlane(const glm::vec3& position, float sphereRadius) const
	{
		if((glm::dot(position, m_Normal) + m_Distance )<= -sphereRadius)
		{
			return false;
		}
		return true;
	}
}
