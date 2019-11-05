/***************************************************************************
 * Filename		: Frustum.cpp
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: Contains 6 planes, for each side of the pyramid view.
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
#include "Frustum.h"
#include "Core/SceneGraph/GameObject.h"
#include "glm/gtc/matrix_access.inl"

namespace Exalted
{

	void Frustum::FromVPMatrix(const glm::mat4& vpMatrix) //todo: Verify this implementation works correctly
	{
		const glm::vec3 xAxis = glm::vec3(glm::row(vpMatrix, 0));
		const glm::vec3 yAxis = glm::vec3(glm::row(vpMatrix, 1));
		const glm::vec3 zAxis = glm::vec3(glm::row(vpMatrix, 2));
		const glm::vec3 wAxis = glm::vec3(glm::row(vpMatrix, 3));

		const glm::vec4 positionVector = glm::vec4(vpMatrix[3]);

		m_Planes[0] = FrustumPlane(wAxis - xAxis, positionVector[3] - positionVector[0], true); //verify this goes to correct indices
		m_Planes[1] = FrustumPlane(wAxis + xAxis, positionVector[3] + positionVector[0], true);
		m_Planes[2] = FrustumPlane(wAxis + yAxis, positionVector[3] + positionVector[1], true);
		m_Planes[3] = FrustumPlane(wAxis - yAxis, positionVector[3] - positionVector[1], true);
		m_Planes[4] = FrustumPlane(wAxis - zAxis, positionVector[3] - positionVector[2], true);
		m_Planes[5] = FrustumPlane(wAxis + zAxis, positionVector[3] + positionVector[2], true);
	}

	// To determine whether a gameobject is inside a view frustum, we assume it is contained within a sphere,
	// so we do a sphere / plane check against each of the 6 planes that make up the frustum. 
	// if the bounding sphere of the gamoebject is outside any of the planes, it cannot be seen, and so should be discarded.
	// only if it is inside all the planes should it be drawn.
	bool Frustum::InsideFrustum(GameObject& gameObject)
	{
		for (int i = 0; i < 6; i++)
		{
			//todo: abstract this so that it calculates bounding volume in plane instead of sphere in plane.
			if(!m_Planes[i].SphereInPlane(glm::vec3((gameObject.GetTransform()->WorldTransform)[3]),gameObject.GetBoundingRadius()))
			{
				return false; // game object is outside plane
			}
		}
		return true; // game object is inside every plane
	}
}
