/***************************************************************************
 * Filename		: GameComponent.h
 * Name			: Ori Lazar
 * Date			: 03/11/2019
 * Description	: Contains the declaration for the game component interface.
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
#pragma once 
#include "Core/Core/Timestep.h"
#include <string>

namespace Exalted
{
	class GameComponent
	{
	public:
		GameComponent(std::string& componentName, std::string&& componentDescription) : m_ComponentName(componentName) {}
		virtual ~GameComponent() { EX_CORE_TRACE("Deleting Game Component: {0}", m_ComponentName); }
		virtual std::string& GetName() { return m_ComponentName; }
		virtual std::string& GetDescription() { return m_Description; }
		virtual void Update(Timestep deltaTime) {} //todo: Don't force implementation, as some game components don't need this, such as the mesh.
		virtual void RemovedFromGameObject() { EX_CORE_TRACE("Removed Game Component: {0}", m_ComponentName); }; //todo: Implement this
	protected:
		std::string m_ComponentName = "Game Component Interface";
		std::string m_Description = "This is an abstract interface for game components.";
	};
}