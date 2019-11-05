/***************************************************************************
 * Filename		: Frustum.h
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
#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "FrustumPlane.h"
#include "Core/SceneGraph/GameObject.h"

//todo: Currently we assume every gameobject exists within a sphere, in the future this should be extended. 

namespace Exalted
{
	class Frustum
	{
	public:
		Frustum() : m_Planes() {}
		~Frustum() = default;
		void FromVPMatrix(const glm::mat4& vpMatrix);
		bool InsideFrustum(GameObject& object);
	private:
		FrustumPlane m_Planes[6];
	};
}