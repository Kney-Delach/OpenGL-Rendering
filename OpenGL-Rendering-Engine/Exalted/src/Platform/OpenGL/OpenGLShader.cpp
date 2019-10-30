/***************************************************************************
 * Filename		: OpenGLShader.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: The implementation of the shader wrapper for OpenGL.
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
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Exalted 
{
	OpenGLShader::OpenGLShader(const std::string& vertexFilePath, const std::string& fragmentFilePath, const std::string& geometryFilePath)
		: m_RendererID(0)
	{
		const std::string vertexSource = ParseShader(vertexFilePath);
		const std::string fragmentSource = ParseShader(fragmentFilePath);
		std::string geometrySource = "";
		if(!geometryFilePath.empty())
			geometrySource = ParseShader(geometryFilePath);

		m_RendererID = CreateShader(vertexSource, fragmentSource, geometrySource);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniformBool1(const std::string& name, bool value)
	{
		glUniform1i(GetUniformLocation(name), static_cast<int>(value));
	}

	void OpenGLShader::SetUniformInt1(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniformFloat1(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2fv(GetUniformLocation(name), 1, &value[0]);
	}

	void OpenGLShader::SetUniformFloat2(const std::string& name, float v0, float v1)
	{
		glUniform2f(GetUniformLocation(name), v0, v1);
	}

	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(GetUniformLocation(name), 1, &value[0]);
	}

	void OpenGLShader::SetUniformFloat3(const std::string& name, float v0, float v1, float v2)
	{
		glUniform3f(GetUniformLocation(name), v0, v1, v2);
	}

	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(GetUniformLocation(name), 1, &value[0]);
	}

	void OpenGLShader::SetUniformFloat4(const std::string& name, float v0, float v1, float v2, float v3)
	{
		glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
	}

	void OpenGLShader::SetUniformMatFloat2(const std::string& name, const glm::mat2& matrix)
	{
		glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	void OpenGLShader::SetUniformMatFloat3(const std::string& name, const glm::mat3& matrix)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	void OpenGLShader::SetUniformMatFloat4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	std::string OpenGLShader::ParseShader(const std::string& filepath) const
	{
		std::ifstream inputFileStream(filepath);
		if(!inputFileStream)
		{
			EX_CORE_CRITICAL("Shader file does not exist!  Path given: {0}", filepath);
			return "";
		}
		std::string line;
		std::stringstream ss;
		ShaderType type = ShaderType::NONE;
		while (getline(inputFileStream, line))
		{
			ss << line << '\n';
		}
		return ss.str();
	}

	uint32_t OpenGLShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
	{
		const unsigned int program = glCreateProgram();
		const unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
		unsigned int gs;
		if (!geometryShader.empty())
		{
			gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
			glAttachShader(program, gs);
		}

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);
		glDeleteShader(vs);
		glDeleteShader(fs);
		if (!geometryShader.empty())
			glDeleteShader(gs);
		
		return program;
	}

	uint32_t OpenGLShader::CompileShader(uint32_t shaderType, const std::string& source)
	{
		unsigned int id = glCreateShader(shaderType);
		const char* src = source.c_str();
		int result;

		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			GLint messageLength;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength);

			std::vector<GLchar> infoLog(messageLength);
			glGetShaderInfoLog(id, messageLength, &messageLength, &infoLog[0]);

			EX_CORE_CRITICAL("{0}", infoLog.data());
			EX_CORE_ERROR("Failed to compile shader! Type: {0}\n------------------------\nShader Source Code:\n------------------------\n{1}", (GL_VERTEX_SHADER ? "VERTEX\n" : GL_FRAGMENT_SHADER ? "FRAGMENT\n" : GL_GEOMETRY_SHADER ? "GEOMETRY\n" : "UNKNOWN SHADER TYPE\n"), source);
			EX_CORE_ASSERT(false, "Shader compilation failure!");

			glDeleteShader(id);
			return 0;
		}
		return id;
	}

	int OpenGLShader::GetUniformLocation(const std::string& uniformName)
	{
		if (m_UniformLocationCache.find(uniformName) != m_UniformLocationCache.end())
			return m_UniformLocationCache[uniformName];
		
		const int location = glGetUniformLocation(m_RendererID, uniformName.c_str());
		if (location == -1)
			EX_CORE_ERROR("Uniform {0} doesn't exist!", uniformName);
		
		m_UniformLocationCache[uniformName] = location;
		return location;
	}
}