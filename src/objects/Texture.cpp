#include "Texture.h"

Texture::Texture(const char *filepath)
	: m_filepath(filepath)
{
	unsigned char *pixels = ImageLoader::load(m_filepath, &m_width, &m_height);
	if (pixels == NULL)
		throw "Could not load image";

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		m_width,
		m_height,
		0,
		GL_BGR,
		GL_UNSIGNED_BYTE,
		pixels
	);
	glBindTexture(GL_TEXTURE_2D, 0);

	ImageLoader::free(pixels);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
