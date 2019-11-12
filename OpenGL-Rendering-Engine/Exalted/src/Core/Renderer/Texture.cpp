/***************************************************************************
 * Filename		: Texture.cpp
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Implementation of a texture wrapper for this engine.
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
#include "Renderer.h"
#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "Platform/OpenGL/OpenGLTextureCube.h"

namespace Exalted
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(width, height);
		}
		EX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& filepath, TextureFormat textureFormat, TextureWrap textureWrap,
		TextureMagFilter textureMagFilter, TextureMinFilter textureMinFilter, bool isRGB, unsigned int mipMapLevel, bool flipTexture)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(filepath, textureFormat, textureWrap, textureMagFilter, textureMinFilter, isRGB, mipMapLevel, flipTexture);
		}
		EX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<TextureCube> TextureCube::Create(const std::vector<std::string>& paths)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTextureCube>(paths);
		}
		EX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	uint32_t Texture::GetBytesPerPixel(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RGB:	return 3;
			case TextureFormat::RGBA:	return 4;
		}
		EX_CORE_ASSERT(false, "Unknown Texture Format!");
		return 0;
	}
}
