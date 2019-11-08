/***************************************************************************
 * Filename		: OpenGLTexture2D.cpp
 * Name			: Ori Lazar
 * Date			: 31/10/2019
 * Description	: This layer contains the implementation of texture loading for the 
 *                OpenGL graphics API.
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
#include "OpenGLTexture2D.h"
#include "Core/Renderer/RendererAPI.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GLFW/glfw3.h>

namespace Exalted
{

	static GLint OpenGLTextureFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB:       return GL_RGB;
		case TextureFormat::RGBA:      return GL_RGBA;
		}
		EX_CORE_ASSERT(false, "Unknown OpenGL Texture Format!");
		EX_CORE_ERROR("Unknown OpenGL Texture Format!");
		return 0;
	}

	static int CalculateMipMapCount(int width, int height)
	{
		int levels = 1;
		while ((width | height) >> levels)
			levels++;
		return levels;
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	//todo: Implement error handling for failing stb read!
	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath, TextureFormat textureFormat, TextureWrap textureWrap, TextureMagFilter textureMagFilter, TextureMinFilter textureMinFilter, bool isRGB, unsigned int mipMapLevel)
		: m_RendererID(0), m_FilePath(filepath), m_TextureFormat(textureFormat),  m_TextureWrap(textureWrap), m_TextureMagFilter(textureMagFilter), m_TextureMinFilter(textureMinFilter)
	{
		EX_CORE_INFO("Loading texture {0}, format {1}, wrapping {2}, magnification filtering {3}, minification filter {4}, isRGB {5}, mipMapLevel {6}", filepath, textureFormat, textureWrap, textureMagFilter, textureMinFilter, isRGB, mipMapLevel);
		
		stbi_set_flip_vertically_on_load(1); //todo: Verify how to set this correctly, as sometime textures need this and some dont....
		int width, height, channels;
		m_LocalDataBuffer = stbi_load(filepath.c_str(), &width, &height, &channels, isRGB ? STBI_rgb : STBI_rgb_alpha);
		m_Transparent = !isRGB; // assign transparency of texture
		if(!m_LocalDataBuffer)
		{
			EX_CORE_CRITICAL("Texture File Path Invalid! Path Given: {0}", filepath.c_str());
			return;
		}
		m_Width = width;
		m_Height = height;

		EX_CORE_INFO("Texture MipMap Capability Level: {0}", CalculateMipMapCount(m_Width, m_Height));
		m_InternalFormat = OpenGLTextureFormat(m_TextureFormat); //todo: Fix this
		m_DataFormat = OpenGLTextureFormat(m_TextureFormat);
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, mipMapLevel, OpenGLTextureFormat(m_TextureFormat), m_Width, m_Height, 0, OpenGLTextureFormat(m_TextureFormat), GL_UNSIGNED_BYTE, m_LocalDataBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);
		SetTextureWrap();
		SetMagFilter();
		SetMinFilter();

		//todo: Make this segment configurable 
		if (!glfwExtensionSupported("GL_EXT_texture_filter_anisotropic") == GLFW_TRUE)
		{
			EX_CORE_ERROR("Anisotropic filtering extension not supported by the current hardware!");
		}
		else
		{
			float amount = 16.f;
			auto& capabilities = RendererAPI::GetCapabilities();
			if (amount < 0 || amount > capabilities.MaxAnisotropy)
				amount = capabilities.MaxAnisotropy;
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0); 
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, amount);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(m_LocalDataBuffer);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGLTexture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		EX_CORE_ASSERT(size == m_Width * m_Height * bpp, "Texture::SetData: Buffer data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::SetTextureWrap() const
	{
		switch (m_TextureWrap)
		{
			case TextureWrap::REPEAT:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				return;
			}
			case TextureWrap::CLAMP:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				return;
			}
		}
		EX_CORE_CRITICAL("Unknown OpenGL Texture Wrapping Format!");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void OpenGLTexture2D::SetMagFilter() const
	{
		switch (m_TextureMagFilter)
		{
			case TextureMagFilter::NEAREST:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				return;
			}
			case TextureMagFilter::LINEAR:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				return;
			}
		}
		EX_CORE_CRITICAL("Unknown OpenGL Texture Magnification Filter Format!");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void OpenGLTexture2D::SetMinFilter() const
	{
		switch (m_TextureMinFilter)
		{
			case TextureMinFilter::NEAREST:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				return;
			}
			case TextureMinFilter::LINEAR:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				return;
			}
			case TextureMinFilter::NEAR_NEAR:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				return;
			}
			case TextureMinFilter::NEAR_LINEAR:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				return;
			}
			case TextureMinFilter::LINEAR_NEAR:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				return;
			}
			case TextureMinFilter::LINEAR_LINEAR:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				return;
			}
		}
		EX_CORE_CRITICAL("Unknown OpenGL Texture Minification Filter Format!");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}
