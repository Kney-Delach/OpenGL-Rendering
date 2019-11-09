/***************************************************************************
 * Filename		: OpenGLTextureCube.h
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: Implementation of an OpenGL texture cube.
                  Note that this implementation currently assumes that the path 
                  of the textures are inserted in the following order:
                  right, left, top, bottom, back, front
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

namespace Exalted
{
	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(const std::vector<std::string>& paths); 
		virtual ~OpenGLTextureCube();

		virtual void Bind(uint32_t slot = 0) const;
		void Unbind() const override;
		void SetData(void* data, uint32_t size) override {} //todo: implement this
		virtual TextureFormat GetFormat() const override { return m_TextureFormat; }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual const std::vector<std::string>& GetPaths() const override { return m_FilePaths; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
	private:
		uint32_t m_RendererID;
		TextureFormat m_TextureFormat;
		uint32_t m_Width;
		uint32_t m_Height;

		unsigned char* m_LocalDataBuffer;

		std::vector<std::string> m_FilePaths;
	};
}