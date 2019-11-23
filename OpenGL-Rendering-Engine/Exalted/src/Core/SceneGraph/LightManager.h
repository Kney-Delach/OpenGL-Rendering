
#include "Core/Renderer/Lights/Light.h"

namespace Exalted
{
	class LightManager
	{
	public:
		LightManager(const Bytes& bufferIndex = 1, const Bytes& lsmBufferIndex = 3, const Bytes& dirLsmBufferIndex = 4) 
			: m_BufferIndex(bufferIndex), m_LsmBufferIndex(lsmBufferIndex), m_DirLsmBufferIndex(dirLsmBufferIndex)
			{}
		~LightManager() = default;

		void AddPointLight(Ref<PointLight>& pointLight) { m_PointLights.emplace_back(pointLight); }
		void AddDirectionalLight(Ref<DirectionalLight>& directionalLight) { m_DirectionalLights.emplace_back(directionalLight); }
		void AddSpotLight(Ref<SpotLight>& spotLight) { m_SpotLights.emplace_back(spotLight); }

		void SetupUniformBuffer()
		{
			Exalted::GameObject::NumberOfSpotLights = m_SpotLights.size();

			Bytes bufferSize = m_PointLights.size() * PointLight::UBSize() + m_DirectionalLights.size() * DirectionalLight::UBSize() + m_SpotLights.size() * SpotLight::UBSize();
			const Bytes offset = 0;
			m_LightUniformBuffer = UniformBuffer::Create(bufferSize);			
			m_LightUniformBuffer->BindBufferRange(m_BufferIndex, offset, bufferSize);

			Bytes lsmBufferSize = m_SpotLights.size() * sizeof(glm::mat4);
			const Bytes lsmOffset = 0;
			m_LsmUniformBuffer = UniformBuffer::Create(lsmBufferSize);			
			m_LsmUniformBuffer->BindBufferRange(m_LsmBufferIndex, lsmOffset, lsmBufferSize);

			Bytes dirLsmBufferSize = m_DirectionalLights.size() * sizeof(glm::mat4);
			const Bytes dirLsmOffset = 0;
			m_DirectionalLsmUniformBuffer = UniformBuffer::Create(dirLsmBufferSize);
			m_DirectionalLsmUniformBuffer->BindBufferRange(m_DirLsmBufferIndex, dirLsmOffset, dirLsmBufferSize);
		}
		void UpdateUniformBufferData() //todo: verify this is the same format as in the shaders
		{
			m_LightUniformBuffer->Bind();
			Bytes lightBufferOffset = 0;
			for (int i = 0; i < m_PointLights.size(); i++)
			{
				m_PointLights[i]->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
			}
			for (int i = 0; i < m_DirectionalLights.size(); i++)
			{
				m_DirectionalLights[i]->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
			}
			for (int i = 0; i < m_SpotLights.size(); i++)
			{
				m_SpotLights[i]->UpdateUniformBuffer(lightBufferOffset, m_LightUniformBuffer);
			}
			m_LightUniformBuffer->Unbind();

			////////////////////////////////////
			//// Spot light light uniforms 
			////////////////////////////////////
			//todo: verify this updates light projection data, update for more than just spot lights
			m_LsmUniformBuffer->Bind();
			Exalted::Bytes offset = 0;
			Exalted::Bytes sizeOfMat4 = sizeof(glm::mat4);
			for (int i = 0; i < m_SpotLights.size(); i++)
			{
				//todo: make this light projection matrix configurable
				glm::mat4 lightProjection = glm::perspective<float>(glm::radians(90.f), 1.0f, 0.1f, 20.f);
				glm::mat4 lightView = glm::lookAt(m_SpotLights[i]->Position, m_SpotLights[i]->Position + m_SpotLights[i]->Direction, glm::vec3(0, 1, 0));
				glm::mat4 lightSpaceMatrix = lightProjection * lightView;
				m_LsmUniformBuffer->SetBufferSubData(offset, sizeOfMat4, glm::value_ptr(lightSpaceMatrix));
				offset += sizeof(glm::mat4);
			}
			m_LsmUniformBuffer->Unbind();

			////////////////////////////////////
			//// Directional light uniforms 
			////////////////////////////////////

			m_DirectionalLsmUniformBuffer->Bind();
			Exalted::Bytes dOffset = 0;

			float ORTHO_SIZE = 100.f;
			for (int i = 0; i < m_DirectionalLights.size(); i++)
			{
				//todo: make this light projection matrix configurable
				glm::mat4 lightProjection = glm::ortho<float>(-150.f, 150.f, -200.f, 400.f, -200.f, 200.f); //todo: make orthographic glm::perspective<float>(glm::radians(90.f), 1.0f, -300.f, 300.f);//
				glm::mat4 lightView = glm::lookAt(m_DirectionalLights[i]->Direction, glm::vec3(0,0,0), glm::vec3(0, 1, 0));
				glm::mat4 lightSpaceMatrix = lightProjection * lightView;
				m_DirectionalLsmUniformBuffer->SetBufferSubData(dOffset, sizeOfMat4, glm::value_ptr(lightSpaceMatrix));
				dOffset += sizeof(glm::mat4);
			}
			m_DirectionalLsmUniformBuffer->Unbind();
		}
	private:
		std::vector<Ref<PointLight>> m_PointLights;
		std::vector<Ref<DirectionalLight>> m_DirectionalLights;
		std::vector<Ref<SpotLight>> m_SpotLights;
		Ref<UniformBuffer> m_LightUniformBuffer;
		Ref<UniformBuffer> m_LsmUniformBuffer;
		Ref<UniformBuffer> m_DirectionalLsmUniformBuffer;
		Bytes m_BufferIndex;
		Bytes m_LsmBufferIndex;
		Bytes m_DirLsmBufferIndex;
	};
}