
#include "Core/Renderer/Lights/Light.h"

namespace Exalted
{
	class LightManager
	{
	public:
		LightManager(const Bytes& bufferIndex) : m_BufferIndex(bufferIndex) {}
		~LightManager() = default;

		void AddPointLight(Ref<PointLight>& pointLight)
		{
			m_PointLights.emplace_back(pointLight);
			//EX_CORE_CRITICAL("Point Light Vector Size: {0}", m_PointLights.size());
		}
		void AddDirectionalLight(Ref<DirectionalLight>& directionalLight) { m_DirectionalLights.emplace_back(directionalLight); }
		void AddSpotLight(Ref<SpotLight>& spotLight) { m_SpotLights.emplace_back(spotLight); }

		void SetupUniformBuffer()
		{
			Bytes bufferSize = m_PointLights.size() * PointLight::UBSize() + m_DirectionalLights.size() * DirectionalLight::UBSize() + m_SpotLights.size() * SpotLight::UBSize();
			const Bytes offset = 0;
			m_LightUniformBuffer = UniformBuffer::Create(bufferSize);			
			m_LightUniformBuffer->BindBufferRange(m_BufferIndex, offset, bufferSize);
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
		}
	private:
		std::vector<Ref<PointLight>> m_PointLights;
		std::vector<Ref<DirectionalLight>> m_DirectionalLights;
		std::vector<Ref<SpotLight>> m_SpotLights;
		Ref<UniformBuffer> m_LightUniformBuffer;
		Bytes m_BufferIndex;
	};
}