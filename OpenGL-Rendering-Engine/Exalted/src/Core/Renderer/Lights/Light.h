/***************************************************************************
 * Filename		: Light.h
 * Name			: Ori Lazar
 * Date			: 13/11/2019
 * Description	: Light structures used to represent a 3D light. 
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

#include "Core/Core.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include "Core/SceneGraph/GameTransform.h"
#include "Core/Renderer/UniformBuffer.h"
#include "ShadowInfo.h"

namespace Exalted
{
	struct Light
	{
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		Ref<ShadowInfo> m_ShadowInfo; // used to specify whether or not light casts shadows
		
		virtual ~Light() = default;
		virtual void UpdateUniformBuffer(Bytes& offset, Ref<UniformBuffer>& ub) {};
		inline Ref<ShadowInfo>& GetShadowInfo() { return m_ShadowInfo; };
	protected:
		Light()
			: Ambient(0), Diffuse(0), Specular(0), m_ShadowInfo(nullptr)
		{
		}

		Light(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
			: Ambient(ambient), Diffuse(diffuse), Specular(specular), m_ShadowInfo(nullptr)
		{
		}

		//void SetShadowInfo(Ref<ShadowInfo>& shadowInfo) { m_ShadowInfo = shadowInfo; }
	};
	
	///////////////////////////////////////////////////////////////////////////////////
	//	PointLight ////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	
	struct PointLight : public Light //todo: Create setters for generic distance attenuation values 
	{
		glm::vec3 Position;

		float AttenuationConstant;
		float AttenuationLinear;
		float AttenuationQuadratic;
		
		PointLight()
			: Light(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)), 
			AttenuationConstant(1), AttenuationLinear(0.7), AttenuationQuadratic(1.8), Position(0)
		{
		}
		PointLight(const glm::vec3& position, 
			const glm::vec3& ambient = glm::vec3(1.0f), const glm::vec3& diffuse = glm::vec3(1.0f), const glm::vec3& specular = glm::vec3(1.0f),
			const float constant = 1.f, const float linear = 0.7, const float quadratic = 1.8)
			: Light(ambient, diffuse, specular), Position(position),
			AttenuationConstant(constant), AttenuationLinear(linear), AttenuationQuadratic(quadratic)
		{
		}

		PointLight(const PointLight& other)
			: Light(other.Ambient, other.Diffuse, other.Specular), Position(other.Position),
			AttenuationConstant(other.AttenuationConstant), AttenuationLinear(other.AttenuationLinear), AttenuationQuadratic(other.AttenuationQuadratic)
		{
		}

		void SetAttenuationDistance(int distance) //todo: optimize this
		{
			switch (distance)
			{
				case 7:
				{
					AttenuationLinear = 0.7f;
					AttenuationQuadratic = 1.8f;
					return;
				}
				case 13:
				{
					AttenuationLinear = 0.35f;
					AttenuationQuadratic = 0.44f;
					return;
				}
				case 20:
				{
					AttenuationLinear = 0.22f;
					AttenuationQuadratic = 0.20f;
					return;
				}
				case 32:
				{
					AttenuationLinear = 0.14f;
					AttenuationQuadratic = 0.07f;
					return;
				}
				case 50:
				{
					AttenuationLinear = 0.09f;
					AttenuationQuadratic = 0.032f;
					return;
				}
				case 65:
				{
					AttenuationLinear = 0.07f;
					AttenuationQuadratic = 0.017f;
					return;
				}
				case 100:
				{
					AttenuationLinear = 0.045f;
					AttenuationQuadratic = 0.0075f;
					return;
				}
				case 160:
				{
					AttenuationLinear = 0.027f;
					AttenuationQuadratic = 0.0028f;
					return;
				}
				case 200:
				{
					AttenuationLinear = 0.022f;
					AttenuationQuadratic = 0.0019f;
					return;
				}
				case 325:
				{
					AttenuationLinear = 0.014;
					AttenuationQuadratic = 0.0007f;
					return;
				}
				case 600:
				{
					AttenuationLinear = 0.007f;
					AttenuationQuadratic = 0.0002f;
					return;
				}
				case 3250:
				{
					AttenuationLinear = 0.0014f;
					AttenuationQuadratic = 0.000007f;
					return;
				}
			}
		}
		virtual ~PointLight() = default;

		virtual void UpdateUniformBuffer(Bytes& offset, Ref<UniformBuffer>& ub) override
		{
				Bytes sizeOfVec4 = sizeof(glm::vec4);
				Bytes sizeOfVec3 = sizeof(glm::vec3);
				Bytes sizeOfFloat = sizeof(float);

				ub->SetBufferSubData(offset, sizeOfVec3, glm::value_ptr(Ambient));
				offset += sizeof(glm::vec3);
				ub->SetBufferSubData(offset, sizeOfFloat, &AttenuationConstant);
				offset += sizeof(float);
				ub->SetBufferSubData(offset, sizeOfVec3, glm::value_ptr(Diffuse));
				offset += sizeof(glm::vec3);
				ub->SetBufferSubData(offset, sizeOfFloat, &AttenuationLinear);
				offset += sizeof(float);
				ub->SetBufferSubData(offset, sizeOfVec3, glm::value_ptr(Specular));
				offset += sizeof(glm::vec3);
				ub->SetBufferSubData(offset, sizeOfFloat, &AttenuationQuadratic);
				offset += sizeof(float);
				ub->SetBufferSubData(offset, sizeOfVec4, glm::value_ptr(Position));
				offset += sizeof(glm::vec4);
		}

		static Bytes UBSize()
		{
			return 4 * sizeof(glm::vec4);
		}

		static Ref<PointLight> Create()
		{
			return Exalted::CreateRef<PointLight>();
		}

		static Ref<PointLight> Create(const glm::vec3& position, 
			const glm::vec3& ambient = glm::vec3(1.0f), const glm::vec3& diffuse = glm::vec3(1.0f), const glm::vec3& specular = glm::vec3(1.0f))
		{
			return Exalted::CreateRef<PointLight>(position ,ambient, diffuse, specular);
		}
	};
	
	///////////////////////////////////////////////////////////////////////////////////
	//	Directional Light /////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	
	struct DirectionalLight : public Light // we set its position as its direction
	{
		glm::vec3 Direction;

		DirectionalLight()
			: Light(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)), Direction(1.f)
		{
			m_ShadowInfo = Exalted::CreateRef<ShadowInfo>(glm::mat4(glm::ortho(-40.f, 40.0f, -40.0f, 40.0f, -40.f, 40.f)));
		}
		DirectionalLight(const glm::vec3& direction, 
			const glm::vec3& ambient = glm::vec3(1.0f), const glm::vec3& diffuse = glm::vec3(1.0f), const glm::vec3& specular = glm::vec3(1.0f))
			: Light(ambient, diffuse, specular), Direction(direction)
		{
			m_ShadowInfo = Exalted::CreateRef<ShadowInfo>(glm::mat4(glm::ortho(-40.f, 40.0f, -40.0f, 40.0f, -40.f, 40.f)));
		}

		DirectionalLight(const DirectionalLight& other)
			: Light(other.Ambient, other.Diffuse, other.Specular), Direction(other.Direction)
		{
			m_ShadowInfo = Exalted::CreateRef<ShadowInfo>(glm::mat4(glm::ortho(-40.f, 40.0f, -40.0f, 40.0f, -40.f, 40.f)));
		}

		virtual ~DirectionalLight() = default;

		virtual void UpdateUniformBuffer(Bytes& offset, Ref<UniformBuffer>& ub) override
		{
			Bytes sizeOfVec4 = sizeof(glm::vec4);
			ub->SetBufferSubData(offset, sizeOfVec4, glm::value_ptr(Ambient));
			offset += sizeof(glm::vec4);
			ub->SetBufferSubData(offset, sizeOfVec4, glm::value_ptr(Diffuse));
			offset += sizeof(glm::vec4);
			ub->SetBufferSubData(offset, sizeOfVec4, glm::value_ptr(Specular));
			offset += sizeof(glm::vec4);
			ub->SetBufferSubData(offset, sizeOfVec4, glm::value_ptr(Direction));
			offset += sizeof(glm::vec4);
		}
		
		static Bytes UBSize()
		{
			return 4 * sizeof(glm::vec4); // 3 for the colors, 1 for the direction
		}

		static Ref<DirectionalLight> Create()
		{
			return Exalted::CreateRef<DirectionalLight>();
		}

		static Ref<DirectionalLight> Create(const glm::vec3& direction, const glm::vec3& ambient = glm::vec3(1.0f), const glm::vec3& diffuse = glm::vec3(1.0f), const glm::vec3& specular = glm::vec3(1.0f))
		{
			return Exalted::CreateRef<DirectionalLight>(direction, ambient, diffuse, specular);
		}
	};
	
	///////////////////////////////////////////////////////////////////////////////////
	//	Spot Light ////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////

	struct SpotLight : public Light
	{
		glm::vec3 Position;
		glm::vec3 Direction;

		float CutoffInner; // specifies the inner cone cutoff. (cos(radians(value_in_degrees)))
		float CutoffOuter; // specifies the outer cone cutoff. (cos(radians(value_in_degrees)))

		float AttenuationConstant;
		float AttenuationLinear;
		float AttenuationQuadratic;
		
		SpotLight()
			: Light(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)),
				Position(1.f), Direction(1.f),
				CutoffInner(glm::cos(glm::radians(12.5f))), CutoffOuter(glm::cos(glm::radians(17.5f))),
				AttenuationConstant(1), AttenuationLinear(0.7), AttenuationQuadratic(1.8)
		{
		}
		SpotLight(const glm::vec3& position, const glm::vec3& direction, 
			const glm::vec3& ambient = glm::vec3(1.0f), const glm::vec3& diffuse = glm::vec3(1.0f), const glm::vec3& specular = glm::vec3(1.0f))
			: Light(ambient, diffuse, specular), Position(position), Direction(direction),
				CutoffInner(glm::cos(glm::radians(12.5f))), CutoffOuter(glm::cos(glm::radians(17.5f))),
				AttenuationConstant(1), AttenuationLinear(0.7), AttenuationQuadratic(1.8)
		{
		}

		SpotLight(const SpotLight& other)
			: Light(other.Ambient, other.Diffuse, other.Specular), Position(other.Position), Direction(other.Direction),
				CutoffInner(other.CutoffInner), CutoffOuter(other.CutoffOuter),
				AttenuationConstant(other.AttenuationConstant), AttenuationLinear(other.AttenuationLinear), AttenuationQuadratic(other.AttenuationQuadratic)
		{
		}

		virtual ~SpotLight() = default;

		virtual void UpdateUniformBuffer(Bytes& offset, Ref<UniformBuffer>& ub) override
		{
			Bytes sizeOfVec3 = sizeof(glm::vec3);
			//Bytes sizeOfVec4 = sizeof(glm::vec4);
			Bytes sizeOfFloat = sizeof(float);

			ub->SetBufferSubData(offset, sizeOfVec3, glm::value_ptr(Ambient));
			offset += sizeof(glm::vec3);
			ub->SetBufferSubData(offset, sizeOfFloat, &CutoffInner);
			offset += sizeof(float);
			ub->SetBufferSubData(offset, sizeOfVec3, glm::value_ptr(Diffuse));
			offset += sizeof(glm::vec3);
			ub->SetBufferSubData(offset, sizeOfFloat, &CutoffOuter);
			offset += sizeof(float);
			ub->SetBufferSubData(offset, sizeOfVec3, glm::value_ptr(Specular));
			offset += sizeof(glm::vec3);
			ub->SetBufferSubData(offset, sizeOfFloat, &AttenuationConstant);
			offset += sizeof(float);
			ub->SetBufferSubData(offset, sizeOfVec3, glm::value_ptr(Position));
			offset += sizeof(glm::vec3);
			ub->SetBufferSubData(offset, sizeOfFloat, &AttenuationLinear);
			offset += sizeof(float);
			ub->SetBufferSubData(offset, sizeOfVec3, glm::value_ptr(Direction));
			offset += sizeof(glm::vec3);
			ub->SetBufferSubData(offset, sizeOfFloat, &AttenuationQuadratic);
			offset += sizeof(float);

		}


		void SetAttenuationDistance(int distance) //todo: optimize this
		{
			switch (distance)
			{
				case 7:
				{
					AttenuationLinear = 0.7f;
					AttenuationQuadratic = 1.8f;
					return;
				}
				case 13:
				{
					AttenuationLinear = 0.35f;
					AttenuationQuadratic = 0.44f;
					return;
				}
				case 20:
				{
					AttenuationLinear = 0.22f;
					AttenuationQuadratic = 0.20f;
					return;
				}
				case 32:
				{
					AttenuationLinear = 0.14f;
					AttenuationQuadratic = 0.07f;
					return;
				}
				case 50:
				{
					AttenuationLinear = 0.09f;
					AttenuationQuadratic = 0.032f;
					return;
				}
				case 65:
				{
					AttenuationLinear = 0.07f;
					AttenuationQuadratic = 0.017f;
					return;
				}
				case 100:
				{
					AttenuationLinear = 0.045f;
					AttenuationQuadratic = 0.0075f;
					return;
				}
				case 160:
				{
					AttenuationLinear = 0.027f;
					AttenuationQuadratic = 0.0028f;
					return;
				}
				case 200:
				{
					AttenuationLinear = 0.022f;
					AttenuationQuadratic = 0.0019f;
					return;
				}
				case 325:
				{
					AttenuationLinear = 0.014;
					AttenuationQuadratic = 0.0007f;
					return;
				}
				case 600:
				{
					AttenuationLinear = 0.007f;
					AttenuationQuadratic = 0.0002f;
					return;
				}
				case 3250:
				{
					AttenuationLinear = 0.0014f;
					AttenuationQuadratic = 0.000007f;
					return;
				}
			}
		}

		static Bytes UBSize()
		{
			return 5 * sizeof(glm::vec4);
		}

		static Ref<SpotLight> Create()
		{
			return Exalted::CreateRef<SpotLight>();
		}

		static Ref<SpotLight> Create(const glm::vec3& position, const glm::vec3& direction, 
			const glm::vec3& ambient = glm::vec3(1.0f), const glm::vec3& diffuse = glm::vec3(1.0f), const glm::vec3& specular = glm::vec3(1.0f))
		{
			return Exalted::CreateRef<SpotLight>(position, direction, ambient, diffuse, specular);
		}
	};
}
