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

namespace Exalted
{
	void Scene::UpdateScene(Timestep deltaTime)
	{
		m_Frustum.FromVPMatrix(m_Camera->GetViewProjectionMatrix());
		m_SceneRoot->Update(deltaTime);
	}

	void Scene::BuildObjectLists(GameObject* gameObject)
	{
		if(m_Frustum.InsideFrustum(*gameObject))
		{
			const glm::vec3 direction = gameObject->GetTransform()->Position - m_Camera->GetPosition(); //todo: verify this works correctly
			gameObject->SetDistanceFromCamera(glm::dot(direction, direction));
			if(gameObject->IsTransparent())
				m_TransparentObjects.push_back(gameObject);
			else
				m_OpaqueObjects.push_back(gameObject);
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

	void Scene::DrawObjectLists()
	{
		for(std::vector<GameObject*>::const_iterator i = m_OpaqueObjects.begin(); i != m_OpaqueObjects.end();++i)
			DrawObject(*i);
		for (std::vector<GameObject*>::const_reverse_iterator i = m_TransparentObjects.rbegin(); i != m_TransparentObjects.rend(); ++i)
			DrawObject(*i);
	}

	void Scene::ClearObjectLists() //todo: Optimization may be removing this? 
	{
		m_OpaqueObjects.clear();
		m_TransparentObjects.clear();
	}
}