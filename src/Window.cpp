#include "Window.h"
#include <string.h>
#include <cstdio>

/**
 * Constructor
 * @param title The window title
 * @param width The width of the window in pixels
 * @param height THe height of the window in pixels
 */
Window::Window(const char *title, int width, int height)
{
	m_width = width;
	m_height = height;

	if (! glfwInit())
		throw "Could not initialize GLFW";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_VISIBLE, 0);

	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (! m_window)
	{
		glfwTerminate();
		throw "Could not create GLFW window";
	}

	glfwSetCursorEnterCallback(m_window, cb_cursor_enter);
	glfwSetCursorPosCallback(m_window, cb_cursor_pos);
	glfwSetMouseButtonCallback(m_window, cb_mouse_button);
	glfwSetKeyCallback(m_window, cb_key);
	glfwSetWindowSizeCallback(m_window, cb_window_size);

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		throw "Could not initialize GLEW";
	}

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

//	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// The window has been created successfully at this point,
	// but on some platforms OpenGL barks about an invalid parameter
	// due to the way GLFW_OPENGL_PROFILE is used... Who cares. It will
	// work, so here call glGetError() without saving the return value
	// because it will reset the error queue.
	glGetError();
}

/**
 * Destructor
 */
Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * Checks for OpenGL errors, updates input events, and draws the window buffer
 */
void Window::update()
{
	GLenum errnum = glGetError();
	if (errnum != GL_NO_ERROR)
	{
		printf("ERROR: %s: OpenGL %d: ", __func__, errnum);
		switch (errnum)
		{
			case GL_INVALID_ENUM:
				printf("Invalid enum\n");
				break;
			case GL_INVALID_VALUE:
				printf("Invalid value\n");
				break;
			case GL_INVALID_OPERATION:
				printf("Invalid operation\n");
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				printf("Invalid framebuffer operation\n");
				break;
			case GL_OUT_OF_MEMORY:
				printf("Out of memory\n");
				break;
			case GL_STACK_UNDERFLOW:
				printf("Stack underflow\n");
				break;
			case GL_STACK_OVERFLOW:
				printf("Stack overflow\n");
				break;
			default:
				printf("\n");
		}
	}

	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

/**
 * Returns whether the window is set to close or not
 * @return Returns TRUE if the window is set to close, else FALSE
 */
bool Window::should_close() const
{
	return (glfwWindowShouldClose(m_window) == 1);
}

/**
 * Cursor enter/exit callback
 * @param window The window that received the event
 * @param entered GLFW_TRUE if the cursor entered the window's client area, or GLFW_FALSE if it left it
 */
void Window::cb_cursor_enter(GLFWwindow *window, int entered)
{
	// TODO Placeholder for now, satifies -Werror=unused-parameters
	if (window && entered) { ; }
}

/**
 * Cursor position callback
 * @param window The window that received the event
 * @param xpos The new cursor x-coordinate, relative to the left edge of the client area
 * @param ypos The new cursor y-coordinate, relative to the top edge of the client area
 */
void Window::cb_cursor_pos(GLFWwindow *window, double xpos, double ypos)
{
	Window *w = (Window *)glfwGetWindowUserPointer(window);

	w->m_cursor_pos.x = xpos;
	w->m_cursor_pos.y = ypos;
}

/**
 * @param window The window that received the event.
 * @param key The keyboard key that was pressed or released.
 * @param scancode The system-specific scancode of the key.
 * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
 * @param mods Bit field describing which modifier keys were held down
 */
void Window::cb_key(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	// TODO Placeholder for now, satifies -Werror=unused-parameters
	if (window && scancode) { ; }

	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS && mods == 0)
				glfwSetWindowShouldClose(window, 1);
			break;
		default:
			break;
	}
}

/**
 * Mouse button callback
 * @param window The window that received the event.
 * @param button The mouse button that was pressed or released.
 * @param action One of GLFW_PRESS or GLFW_RELEASE.
 * @param mods Bit field describing which modifier keys were held down
 */
void Window::cb_mouse_button(GLFWwindow *window, int button, int action, int mods)
{
	// TODO Placeholder for now, satifies -Werror=unused-parameters
	if (window && button && action && mods) { ; }
}

/**
 * Window (re)size callback
 * @param window The window that was resized
 * @param width The new width, in screen coordinates, of the window
 * @param height The new height, in screen coordinates, of the window
 */
void Window::cb_window_size(GLFWwindow *window, int width, int height)
{
	Window *w = (Window *)glfwGetWindowUserPointer(window);

	glViewport(0, 0, width, height);
	w->m_width = width;
	w->m_height = height;
}

/**
 * Returns a double representing the current timestamp
 * @return
 */
double Window::get_time()
{
	return glfwGetTime();
}

