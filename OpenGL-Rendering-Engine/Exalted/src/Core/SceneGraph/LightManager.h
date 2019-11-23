
#include "Core/Renderer/Lights/Light.h"

namespace Exalted
{
	class LightManager
	{
	public:
		LightManager(const Bytes& bufferIndex = 1, const Bytes& lsmBufferIndex = 3) 
			: m_BufferIndex(bufferIndex), m_LsmBufferIndex(lsmBufferIndex) 
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

			Bytes lsmBufferSize = m_SpotLights.size() * sizeof(glm::mat4); //todo: add data for more than just spot lights
			const Bytes lsmOffset = 0;
			m_LsmUniformBuffer = UniformBuffer::Create(lsmBufferSize);			
			m_LsmUniformBuffer->BindBufferRange(m_LsmBufferIndex, lsmOffset, lsmBufferSize);
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
		}
	private:
		std::vector<Ref<PointLight>> m_PointLights;
		std::vector<Ref<DirectionalLight>> m_DirectionalLights;
		std::vector<Ref<SpotLight>> m_SpotLights;
		Ref<UniformBuffer> m_LightUniformBuffer;
		Ref<UniformBuffer> m_LsmUniformBuffer;
		Bytes m_BufferIndex;
		Bytes m_LsmBufferIndex;
	};
}