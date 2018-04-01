#ifndef ECE_6122_FINAL_PROJECT_WINDOW_H
#define ECE_6122_FINAL_PROJECT_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window
{
public:
	Window(const char *title, int width, int height);
	~Window();

	void clear();
	void update();
	double get_time();

	bool should_close() const;

	inline int get_width() const { return m_width; }
	inline int get_height() const { return m_height; }
	inline glm::vec2 get_cursor_pos() const { return m_cursor_pos; }

private:

	GLFWwindow *m_window;
	int m_width;
	int m_height;
	glm::dvec2 m_cursor_pos;

	static void cb_cursor_enter(GLFWwindow *window, int entered);
	static void cb_cursor_pos(GLFWwindow *window, double xpos, double ypos);
	static void cb_mouse_button(GLFWwindow *window, int button, int action, int mods);
	static void cb_key(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void cb_window_size(GLFWwindow *window, int width, int height);
};


#endif //ECE_6122_FINAL_PROJECT_WINDOW_H
