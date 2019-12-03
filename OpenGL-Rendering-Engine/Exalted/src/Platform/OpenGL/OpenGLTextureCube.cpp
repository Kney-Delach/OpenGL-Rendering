#include "expch.h"
#include "OpenGLTextureCube.h"

#include "glad/glad.h"
#include <stb_image.h>
#include "Core/Renderer/RendererAPI.h"

namespace Exalted
{
	OpenGLTextureCube::OpenGLTextureCube(const std::vector<std::string>& paths)
		: m_FilePaths(paths)
	{
		stbi_set_flip_vertically_on_load(0);
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		int width, height, channels;
		for (GLuint i = 0; i < paths.size(); i++)
		{
			unsigned char* data = stbi_load(m_FilePaths[i].c_str(), &width, &height, &channels, 0);
			if(data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				EX_CORE_WARN("Failed to load cube texture at path: {0}", m_FilePaths[i].c_str());
				stbi_image_free(data);
			}
		}

		//todo: Make these configurable
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, 0);
		glTextureParameterf(m_RendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureCube::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}

	void OpenGLTextureCube::Unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}