/***************************************************************************
 * Filename		: Scene.cpp
 * Name			: Ori Lazar
 * Date			: 05/11/2019
 * Description	: A container for a scene.
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
#include "Scene.h"
#include "Core/Events/KeyEvent.h"
#include "Core/KeyCodes.h"
#include "Platform/OpenGL/OpenGLConfigurations.h" //todo: Abstract this into just "Configurations"

namespace Exalted
{
	void Scene::UpdateScene(Timestep deltaTime)
	{
		m_Frustum.FromVPMatrix(m_Camera->GetViewProjectionMatrix());
		m_SceneRoot->Update(deltaTime);
	}

	void Scene::DrawOpaqueObjects()
	{
		for (std::vector<GameObject*>::const_iterator i = m_OpaqueObjects.begin(); i != m_OpaqueObjects.end(); ++i)
			DrawObject(*i);
	}

	void Scene::DrawTransparentObjects()
	{
		for (std::vector<GameObject*>::const_reverse_iterator i = m_TransparentObjects.rbegin(); i != m_TransparentObjects.rend(); ++i)
			DrawObject(*i);
	}

	void Scene::DrawOpaqueBindless(const Ref<Shader>& shadowShader)
	{
		for (std::vector<GameObject*>::const_iterator i = m_OpaqueObjects.begin(); i != m_OpaqueObjects.end(); ++i)
			DrawObjectBindless(*i, shadowShader);
	}

	void Scene::DrawTransparentBindless(const Ref<Shader>& shadowShader)
	{
		for (std::vector<GameObject*>::const_reverse_iterator i = m_TransparentObjects.rbegin(); i != m_TransparentObjects.rend(); ++i)
			DrawObjectBindless(*i, shadowShader);
	}
	
	void Scene::BuildObjectLists(GameObject* gameObject)
	{
		if (!(gameObject->IsActive()))
			return;
		if(m_Frustum.InsideFrustum(*gameObject))
		{
			const glm::vec3 direction = glm::vec3(gameObject->GetTransform()->WorldTransform[3]) - m_Camera->GetPosition(); //todo: verify this works correctly gameObject->GetTransform()->Position
			gameObject->SetDistanceFromCamera(glm::dot(direction, direction));
			if(gameObject->IsTransparent())
				m_TransparentObjects.emplace_back(gameObject);
			else
				m_OpaqueObjects.emplace_back(gameObject);
		} 

		for(std::vector<GameObject*>::const_iterator i = gameObject->GetChildIteratorStart(); i != gameObject->GetChildIteratorEnd(); ++i)
			BuildObjectLists(*i);
	}

	void Scene::SortObjectLists()
	{
		std::sort(m_TransparentObjects.begin(), 
			m_TransparentObjects.end(), 
			GameObject::CompareByCameraDistance);
		std::sort(m_OpaqueObjects.begin(),
			m_OpaqueObjects.end(),
			GameObject::CompareByCameraDistance);
	}

	void Scene::DrawObjectLists() //todo: Draw skybox inbetween here? 
	{
		for(std::vector<GameObject*>::const_iterator i = m_OpaqueObjects.begin(); i != m_OpaqueObjects.end();++i)
			DrawObject(*i);
		RenderSkybox(); //todo: verify this doesn't break everything
		for (std::vector<GameObject*>::const_reverse_iterator i = m_TransparentObjects.rbegin(); i != m_TransparentObjects.rend(); ++i)
			DrawObject(*i);
	}

	void Scene::ClearObjectLists() //todo: Optimization may be removing this? 
	{
		m_OpaqueObjects.clear();
		m_TransparentObjects.clear();
	}

	void Scene::OnEvent(Exalted::Event& event)
	{
		if (event.GetEventType() == Exalted::EventType::KeyPressed)
		{
			auto& e = static_cast<Exalted::KeyPressedEvent&>(event);
			if (e.GetKeyCode() == EX_KEY_I)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::POINT);

			if (e.GetKeyCode() == EX_KEY_O)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::LINE);

			if (e.GetKeyCode() == EX_KEY_P)
				Exalted::OpenGLConfigurations::SetPolygonMode(Exalted::FILL);
		}
	}
}