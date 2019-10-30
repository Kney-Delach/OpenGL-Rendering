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

//todo: Replace glm implementation with Mathex math library.

namespace Exalted 
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		_NODISCARD inline uint32_t* GetRendererID() { return &m_RendererID; }

		void SetUniformBool1(const std::string& name, bool value);
		void SetUniformInt1(const std::string& name, int value);
		void SetUniformFloat1(const std::string& name, float value);

		void SetUniformFloat2(const std::string& name, const glm::vec2& value);
		void SetUniformFloat2(const std::string& name, float v0, float v1);

		void SetUniformFloat3(const std::string& name, const glm::vec3& value);
		void SetUniformFloat3(const std::string& name, float v0, float v1, float v2);

		void SetUniformFloat4(const std::string& name, const glm::vec4& value);
		void SetUniformFloat4(const std::string& name, float v0, float v1, float v2, float v3);

		void SetUniformMatFloat2(const std::string& name, const glm::mat2& matrix);

		void SetUniformMatFloat3(const std::string& name, const glm::mat3& matrix);

		void SetUniformMatFloat4(const std::string& name, const glm::mat4& matrix);
	private:
		ShaderProgramSource ParseShader(const std::string& filepath) const;
		uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		uint32_t CompileShader(uint32_t shaderType, const std::string& source);

		int GetUniformLocation(const std::string& uniformName);
	private:
		std::string m_FilePath;
		uint32_t m_RendererID;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}