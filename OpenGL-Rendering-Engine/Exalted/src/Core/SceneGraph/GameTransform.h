/***************************************************************************
 * Filename		: GameTransform.h
 * Name			: Ori Lazar
 * Date			: 03/11/2019
 * Description	: Contains a transform component of a game object.
                  Include Mathex here eventually.
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

// todo: Implement local rotation options + recalculate local transform from gui render function (to be implemented)

namespace Exalted
{
	struct GameTransform
	{
		glm::mat4 WorldTransform;
		glm::mat4 LocalTransform;
		glm::vec3 ScaleTransform;

		GameTransform() 
		: WorldTransform(glm::mat4(1.f)), LocalTransform(glm::mat4(1.f)), ScaleTransform( glm::vec3(1.f))
		{}

		void SetWorldTransform(glm::mat4& worldTransform) { WorldTransform = worldTransform; }
		glm::mat4& GetWorldTransform() { return WorldTransform; }

		void SetLocalTransform(glm::mat4& localTransform) { LocalTransform = localTransform; }
		glm::mat4& GetLocalTransform() { return LocalTransform; }

		void SetScaleTransform(glm::vec3& scaleTransform)
		{
			ScaleTransform = scaleTransform;
			LocalTransform = glm::scale(LocalTransform, ScaleTransform);
		}
		glm::vec3& GetScaleTransform() { return ScaleTransform; }
	};
}