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

namespace Exalted
{
	Texture2D* Texture2D::Create(const std::string& filepath, TextureFormat textureFormat, TextureWrap textureWrap,
		TextureMagFilter textureMagFilter, TextureMinFilter textureMinFilter, bool isRGB, unsigned int mipMapLevel)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLTexture2D(filepath, textureFormat, textureWrap, textureMagFilter, textureMinFilter, isRGB, mipMapLevel);
		}
		return nullptr;
	}

	uint32_t Texture::GetBytesPerPixel(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB:    return 3;
		case TextureFormat::RGBA:   return 4;
		}
		return 0;
	}
}