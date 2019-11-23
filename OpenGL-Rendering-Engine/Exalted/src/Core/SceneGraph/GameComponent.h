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
		GameComponent(Ref<GameTransform>& transform, const std::string& componentName, const std::string& componentDescription) : m_Transform(transform), m_ComponentName(componentName), m_Description(componentDescription) {}
		virtual ~GameComponent() { EX_CORE_TRACE("Deleting Game Component: {0}", m_ComponentName); }
		virtual std::string& GetName() { return m_ComponentName; }
		virtual std::string& GetDescription() { return m_Description; }
		virtual void Update(Timestep deltaTime) = 0;
		virtual void RemovedFromGameObject() { EX_CORE_TRACE("Removed Game Component: {0}", m_ComponentName); }; //todo: Implement this
	protected:
		Ref<GameTransform> m_Transform;
		std::string m_ComponentName = "Game Component Interface";
		std::string m_Description = "This is an abstract interface for game components.";
		//todo: implement interface to communicate to gameobject parent when done with task so can be removed.
	};

	/////////////////////////////////////////////
	//// Rotation
	/////////////////////////////////////////////
	class RotateComponent : public GameComponent
	{
	public:
		RotateComponent(Ref<GameTransform>& transform, const glm::vec3& rotationRate) : GameComponent(transform,"Rotate Component", "Rotates a gameobject over time"), m_RotationRate(rotationRate) {}
		virtual ~RotateComponent() {}
		void Update(Timestep deltaTime) override
		{
			m_Transform->Rotation = glm::mod((m_Transform->Rotation + m_RotationRate * deltaTime.GetSeconds()), glm::vec3(360.f));
		}
	protected:
		glm::vec3 m_RotationRate;
	};

	/////////////////////////////////////////////
	//// Translation
	/////////////////////////////////////////////
	class TranslateComponent : public GameComponent
	{
	public:
		TranslateComponent(Ref<GameTransform>& transform, const glm::vec3& translationRate) : GameComponent(transform,"Translate Component", "Translates a gameobject over time"), m_TranslationRate(translationRate) {}
		virtual ~TranslateComponent() {}
		void Update(Timestep deltaTime) override
		{
			m_Transform->Position = m_Transform->Position + m_TranslationRate * deltaTime.GetSeconds();
		}
	protected:
		glm::vec3 m_TranslationRate;
	};

	/////////////////////////////////////////////
	//// Scaling
	/////////////////////////////////////////////
	class ScaleComponent : public GameComponent
	{
	public:
		ScaleComponent(Ref<GameTransform>& transform, const glm::vec3& scaleRate) : GameComponent(transform, "Scale Component", "Scaled a gameobject over time"), m_ScaleRate(scaleRate) {}
		virtual ~ScaleComponent() {}
		void Update(Timestep deltaTime) override
		{
			m_Transform->Scale = m_Transform->Scale + m_ScaleRate * deltaTime.GetSeconds();
		}
	protected:
		glm::vec3 m_ScaleRate;
	};
	
	//// Max Scaler
	class ScaleGrowComponent : public GameComponent
	{
	public:
		ScaleGrowComponent(Ref<GameTransform>& transform, const glm::vec3& scaleRate, const glm::vec3& maxScale) : GameComponent(transform, "Scale Grow Component", "Scales a gameobject over time until a limited size"),
			m_ScaleRate(scaleRate), m_MaxScale(maxScale) {}
		virtual ~ScaleGrowComponent() {}
		void Update(Timestep deltaTime) override
		{
			if(!(m_Transform->Scale.x >= m_MaxScale.x || m_Transform->Scale.y >= m_MaxScale.y || m_Transform->Scale.z >= m_MaxScale.z))
				m_Transform->Scale = m_Transform->Scale + m_ScaleRate * deltaTime.GetSeconds(), m_MaxScale;
		}
	protected:
		glm::vec3 m_ScaleRate;
		glm::vec3 m_MaxScale;
	};
}