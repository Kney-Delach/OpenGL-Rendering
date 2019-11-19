#pragma once
#include "glm/mat4x4.hpp"

namespace Exalted
{
	class ShadowInfo
	{
	public:
		ShadowInfo(const glm::mat4& projection) : m_Projection(projection) {}

		inline glm::mat4 GetProjection() { return m_Projection; }
	protected:

	private:
		glm::mat4 m_Projection; // projection matrix used for rendering shadow map
	};
}
