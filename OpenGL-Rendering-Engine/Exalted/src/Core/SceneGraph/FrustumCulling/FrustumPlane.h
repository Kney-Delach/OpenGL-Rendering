/***************************************************************************
 * Filename		: FrustumPlane.h
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
#pragma once
#include <glm/gtc/matrix_transform.hpp>

namespace Exalted
{
	class FrustumPlane
	{
	public:
		FrustumPlane() = default;
		FrustumPlane(const glm::vec3& normalVector, float distanceFromOrigin, bool normalize = false);
		~FrustumPlane() = default;

		void SetNormal(const glm::vec3& planeNormal) {}
		glm::vec3 GetNormal() const { return m_Normal; } //todo: optimize 

		void SetDistance(const float distanceFromOrigin) { m_Distance = distanceFromOrigin; }
		float GetDistance() const { return m_Distance; } //todo: optimize 

		bool SphereInPlane(const glm::vec3& position, float sphereRadius) const;
	protected:
		glm::vec3 m_Normal;
		float m_Distance;
	};
}