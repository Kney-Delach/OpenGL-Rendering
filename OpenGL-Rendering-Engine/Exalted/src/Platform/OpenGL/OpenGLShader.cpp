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


namespace Exalted 
{
	OpenGLShader::OpenGLShader(const std::string& vertexFilePath, const std::string& fragmentFilePath, const std::string& geometryFilePath, const std::string& tessEvalFilePath, const std::string& tessControlFilePath)
		: m_RendererID(0) // Shader("OpenGL Shader"), 
	{
		const std::string vertexSource = ParseShader(vertexFilePath);
		const std::string fragmentSource = ParseShader(fragmentFilePath);
		
		std::string geometrySource = "";
		if(!geometryFilePath.empty())
			geometrySource = ParseShader(geometryFilePath);

		std::string tessEvalSource = "";
		if (!tessEvalFilePath.empty())
			tessEvalSource = ParseShader(tessEvalFilePath);

		std::string tessControlSource = "";
		if (!tessControlFilePath.empty())
			tessControlSource = ParseShader(tessControlFilePath);

		m_RendererID = CreateShader(vertexSource, fragmentSource, geometrySource, tessEvalSource, tessControlSource);
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

	uint32_t OpenGLShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader, const std::string& tessEvalShader, const std::string& tessControlShader)
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
		unsigned int tcs;
		if (!tessControlShader.empty())
		{
			tcs = CompileShader(GL_TESS_CONTROL_SHADER, tessControlShader);
			glAttachShader(program, tcs);
		}
		unsigned int tes;
		if (!tessEvalShader.empty())
		{
			tes = CompileShader(GL_TESS_EVALUATION_SHADER, tessEvalShader);
			glAttachShader(program, tes);
		}

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);
		glDeleteShader(vs);
		glDeleteShader(fs);
		if (!geometryShader.empty())
			glDeleteShader(gs);
		if (!tessEvalShader.empty())
			glDeleteShader(tes);
		if (!tessControlShader.empty())
			glDeleteShader(tcs);
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
			EX_CORE_ERROR("Failed to compile shader! Type: {0}\n------------------------\nShader Source Code:\n------------------------\n{1}", (GL_VERTEX_SHADER ? "VERTEX\n" : GL_FRAGMENT_SHADER ? "FRAGMENT\n" : GL_GEOMETRY_SHADER ? "GEOMETRY\n" : GL_TESS_CONTROL_SHADER ? "TESSELATION CONTROL\n" : GL_TESS_EVALUATION_SHADER ? "TESSELATION EVALUATION\n" : "UNKNOWN SHADER TYPE\n"), source);
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