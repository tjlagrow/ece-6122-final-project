#ifndef ECE_6122_FINAL_PROJECT_TEXTURE_H
#define ECE_6122_FINAL_PROJECT_TEXTURE_H

#include "../utils/ImageLoader.h"
#include <GL/glew.h>
#include <string>

class Texture
{
public:
	Texture(const char *filepath);
	~Texture();

	void bind() const;
	void unbind() const;

	inline GLsizei get_width() const { return m_width; }
	inline GLsizei get_height() const { return m_height; }
	inline GLuint get_id() const { return m_id; }

private:
	GLuint load();

	const char *m_filepath;
	GLuint m_id;
	GLsizei m_width, m_height;

};


#endif //ECE_6122_FINAL_PROJECT_TEXTURE_H
