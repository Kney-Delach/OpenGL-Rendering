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

//todo: Optimize this class

namespace Exalted
{
	struct GameTransform
	{
		glm::mat4 WorldTransform;
		glm::vec3 Position; 
		glm::vec3 Scale;
		glm::vec3 Rotation; 

		GameTransform() 
		: 
		WorldTransform(glm::mat4(1.f)), 
		Position(glm::vec3(0.f)), 
		Scale(glm::vec3(1.f)), 
		Rotation(glm::vec3(0.f))
		{}

		__forceinline void SetWorldTransform()
		{
			glm::mat4 temporaryTransform(1.f); //todo: verify this works correctly. 
			temporaryTransform = glm::translate(temporaryTransform, Position);
			temporaryTransform = glm::rotate(temporaryTransform, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
			temporaryTransform = glm::rotate(temporaryTransform, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
			temporaryTransform = glm::rotate(temporaryTransform, glm::radians(Rotation.z), glm::vec3(0,0,1)); //todo: maybe this should be -1
			WorldTransform = glm::scale(temporaryTransform, Scale);
		}
		__forceinline void SetWorldTransform(glm::mat4& worldTransform)
		{
			SetWorldTransform();
			const glm::mat4 tempTransform = worldTransform * WorldTransform;
			WorldTransform = tempTransform;
		}
		glm::mat4& GetWorldTransform() { return WorldTransform; }
	};
}