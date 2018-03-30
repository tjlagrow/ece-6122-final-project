#include "Shader.h"
#include <string>
#include <vector>
#include <cstdio>

std::string read_file(const char *filepath);

/**
 * Constructor
 * @param vert_path File path to the vertex shader
 * @param frag_path File path to the fragment shader
 */
Shader::Shader(const char *vert_path, const char *frag_path)
	: m_vertpath(vert_path), m_fragpath(frag_path)
{
	m_id = load();
}

/**
 * Destructor
 */
Shader::~Shader()
{
	glDeleteProgram(m_id);
}

/**
 * Enables use of the shader program
 */
void Shader::enable() const
{
	glUseProgram(m_id);
}

/**
 * Disables use of the shader program
 */
void Shader::disable() const
{
	glUseProgram(0);
}

/**
 * TODO
 * @param name TODO
 * @param val TODO
 */
void Shader::setUniform1f(const GLchar *name, const float val)
{
	glUniform1f(getUniformLocation(name), val);
}

/**
 * TODO
 * @param name TODO
 * @param val TODO
 */
void Shader::setUniform1i(const GLchar *name, const int val)
{
	glUniform1i(getUniformLocation(name), val);
}

/**
 * TODO
 * @param name TODO
 * @param val TODO
 */
void Shader::setUniform2f(const GLchar *name, const glm::vec2 &val)
{
	glUniform2f(getUniformLocation(name), val.x, val.y);
}

/**
 * TODO
 * @param name TODO
 * @param val TODO
 */
void Shader::setUniform3f(const GLchar *name, const glm::vec3 &val)
{
	glUniform3f(getUniformLocation(name), val.x, val.y, val.z);
}

/**
 * TODO
 * @param name TODO
 * @param val TODO
 */
void Shader::setUniform4f(const GLchar *name, const glm::vec4 &val)
{
	float farr[4];
	const float *pSource = (const float *)glm::value_ptr(val);
	for (int i = 0; i < 4; ++i)
		farr[i] = pSource[i];
	glUniform4f(getUniformLocation(name), farr[0], farr[1], farr[2], farr[3]);
}

/**
 * TODO
 * @param name TODO
 * @param val TODO
 */
void Shader::setUniformMat4(const GLchar *name, const glm::mat4& val)
{
//	float farr[16] = {0.0};
//	const float *pSource = (const float *)glm::value_ptr(val);
//	for (int i = 0; i < 16; ++i)
//		farr[i] = pSource[i];
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &val[0][0]);
}

/**
 * TODO
 * @return TODO
 */
GLuint Shader::load()
{
	GLint status;
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint program = glCreateProgram();

	std::string vert_stdstr = read_file(m_vertpath);
	std::string frag_stdstr = read_file(m_fragpath);

	const char *vert_cstr = vert_stdstr.c_str();
	const char *frag_cstr = frag_stdstr.c_str();

	glShaderSource(vertex_shader, 1, &vert_cstr, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> errmsg(length);
		glGetShaderInfoLog(vertex_shader, length, &length, &errmsg[0]);
		printf("ERROR: Failed to compile vertex shader:\n%s\n", errmsg.data());
		glDeleteShader(vertex_shader);
		return (0);
	}

	glShaderSource(fragment_shader, 1, &frag_cstr, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> errmsg(length);
		glGetShaderInfoLog(fragment_shader, length, &length, &errmsg[0]);
		printf("ERROR: Failed to compile fragment shader:\n%s\n", errmsg.data());
		glDeleteShader(fragment_shader);
		return (0);
	}

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);

	return program;
}

/**
 * TODO
 * @param name TODO
 * @return TODO
 */
GLint Shader::getUniformLocation(const GLchar *name)
{
	GLint loc = glGetUniformLocation(m_id, name);
	return (loc);
}

/**
 * TODO
 * @param filepath TODO
 * @return TODO
 */
std::string read_file(const char *filepath)
{
	FILE *fp;
	unsigned long nbytes;
	char *raw;

	if ((fp = fopen(filepath, "r")) == NULL)
	{

		throw std::string("Could not open file: %s\n", filepath);
		return "";
	}

	fseek(fp, 0, SEEK_END);
	nbytes = ftell(fp);
	if ((raw = (char *) calloc(1, nbytes)) == NULL)
	{
		fclose(fp);
		return "";
	}
	fseek(fp, 0, SEEK_SET);
	fread(raw, 1, nbytes, fp);
	fclose(fp);

	std::string buf(raw);
	free(raw);
	return (buf);
}
