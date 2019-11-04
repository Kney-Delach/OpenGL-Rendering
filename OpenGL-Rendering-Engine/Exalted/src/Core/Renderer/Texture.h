/***************************************************************************
 * Filename		: Texture.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: A declaration of a texture wrapper for this engine.
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
#include <string>

//todo: Set read and storage type configurable seperatly (rgb vs rgba) 

namespace Exalted 
{
	enum class TextureFormat { RGB = 1, RGBA = 2 };
	enum class TextureWrap { CLAMP = 0, REPEAT = 1 };
	enum class TextureMagFilter { NEAREST = 0, LINEAR = 1 };
	enum class TextureMinFilter
	{
		NEAREST = 0, LINEAR = 1,
		NEAR_NEAR = 2, NEAR_LINEAR = 3, LINEAR_NEAR = 4, LINEAR_LINEAR = 5
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		static uint32_t GetBytesPerPixel(TextureFormat format);
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& filepath, TextureFormat textureFormat, TextureWrap textureWrap = TextureWrap::REPEAT, 
			TextureMagFilter textureMagFilter = TextureMagFilter::LINEAR, TextureMinFilter textureMinFilter = TextureMinFilter::LINEAR, bool isRGB = false, unsigned int mipMapLevel = 0);
		_NODISCARD virtual TextureFormat GetFormat() const = 0;
		_NODISCARD virtual uint32_t GetWidth() const = 0;
		_NODISCARD virtual uint32_t GetHeight() const = 0;
		_NODISCARD virtual const std::string& GetPath() const = 0;
	};
}