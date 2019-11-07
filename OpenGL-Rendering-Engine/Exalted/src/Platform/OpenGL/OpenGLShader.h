/***************************************************************************
 * Filename		: OpenGLShader.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: A declaration of a shader wrapper for OpenGL.
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
#include "Core/Renderer/Shader.h"
#include <unordered_map>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


//todo: Replace glm implementation with Mathex math library.

namespace Exalted 
{
	enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2 };

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexFilePath, const std::string& fragmentFilePath, const std::string& geometryFilePath = "");
		_NODISCARD inline uint32_t* GetRendererID() { return &m_RendererID; }

		__forceinline virtual ~OpenGLShader() override
		{
			glDeleteProgram(m_RendererID);
		}

		__forceinline virtual void Bind() const override
		{
			glUseProgram(m_RendererID);
		}

		__forceinline virtual void Unbind() const override
		{
			glUseProgram(0);
		}

		__forceinline void SetUniformBool1(const std::string& name, bool value)
		{
			glUniform1i(GetUniformLocation(name), static_cast<int>(value));
		}

		__forceinline void SetUniformInt1(const std::string& name, int value)
		{
			glUniform1i(GetUniformLocation(name), value);
		}

		__forceinline void SetUniformFloat1(const std::string& name, float value)
		{
			glUniform1f(GetUniformLocation(name), value);
		}

		__forceinline void SetUniformFloat2(const std::string& name, const glm::vec2& value)
		{
			glUniform2fv(GetUniformLocation(name), 1, &value[0]);
		}

		__forceinline void SetUniformFloat2(const std::string& name, float v0, float v1)
		{
			glUniform2f(GetUniformLocation(name), v0, v1);
		}

		__forceinline void SetUniformFloat3(const std::string& name, const glm::vec3& value)
		{
			glUniform3fv(GetUniformLocation(name), 1, &value[0]);
		}

		__forceinline void SetUniformFloat3(const std::string& name, float v0, float v1, float v2)
		{
			glUniform3f(GetUniformLocation(name), v0, v1, v2);
		}

		__forceinline void SetUniformFloat4(const std::string& name, const glm::vec4& value)
		{
			glUniform4fv(GetUniformLocation(name), 1, &value[0]);
		}

		__forceinline void SetUniformFloat4(const std::string& name, float v0, float v1, float v2, float v3)
		{
			glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
		}

		__forceinline void SetUniformMatFloat2(const std::string& name, const glm::mat2& matrix)
		{
			glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
		}

		__forceinline void SetUniformMatFloat3(const std::string& name, const glm::mat3& matrix)
		{
			glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
		}

		__forceinline void SetUniformMatFloat4(const std::string& name, const glm::mat4& matrix)
		{
			glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
		}
	private:
		_NODISCARD std::string ParseShader(const std::string& filepath) const;
		uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
		uint32_t CompileShader(uint32_t shaderType, const std::string& source);

		int GetUniformLocation(const std::string& uniformName);
	private:
		uint32_t m_RendererID;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}