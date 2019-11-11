/***************************************************************************
 * Filename		: OpenGLTexture2D.h
 * Name			: Ori Lazar
 * Date			: 31/10/2019
 * Description	: Contains the 2D texture wrapping declaration for the OpenGL api.
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
#include "Core/Renderer/Texture.h"

#include <glad/glad.h>

namespace Exalted
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& filepath, TextureFormat textureFormat, TextureWrap textureWrap = TextureWrap::REPEAT, 
			TextureMagFilter textureMagFilter = TextureMagFilter::LINEAR, TextureMinFilter textureMinFilter = TextureMinFilter::LINEAR, bool isRGB = false, unsigned int mipMapLevel = 0);
		virtual ~OpenGLTexture2D();
		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;
		void SetData(void* data, uint32_t size) override;
		_NODISCARD virtual TextureFormat GetFormat() const override { return m_TextureFormat; }
		_NODISCARD virtual uint32_t GetWidth() const override { return m_Width; }
		_NODISCARD virtual uint32_t GetHeight() const override { return m_Height; }
		_NODISCARD virtual const std::string& GetPath() const override { return m_FilePath; }
		_NODISCARD virtual uint32_t GetRendererID() const override { return m_RendererID; }
	private:
		void SetTextureWrap() const;
		void SetMagFilter() const;
		void SetMinFilter() const;
	private:
		uint32_t m_RendererID;
		std::string m_FilePath;
		TextureFormat m_TextureFormat;
		TextureWrap m_TextureWrap;
		TextureMagFilter m_TextureMagFilter;
		TextureMinFilter m_TextureMinFilter;
		uint32_t m_Width;
		uint32_t m_Height;
		unsigned char* m_LocalDataBuffer;
		GLenum m_InternalFormat, m_DataFormat;
	};
}