#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLContext.h"
#include "core/Window.h"
#include "input/InputManager.h"

//=============================================================================//
// ======================   Callbacks functions   ============================//
static void on_window_size_callback(GLFWwindow* window, int width, int height)
{
	auto pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	pWindow->on_resize(width, height);
}

static void on_window_close_callback(GLFWwindow* window)
{
	auto* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	pWindow->on_close();
}
//------------
static void on_mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	auto* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	pWindow->getInputManager()->handleMouseButtonEvent(button, action);
}

static void on_cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	pWindow->getInputManager()->handleCursorPositionEvent(xpos, ypos);
}

static void on_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto pwindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	pwindow->getInputManager()->handleKeyEvent(key, action);
}

//=============================================================================//


GLContext::GLContext(Window* win): window(win) {

}

bool GLContext::init() {
	/* Initialize GLFW */
	if (!glfwInit())
	{
		printf("Error: GLFW Window couldn't be initialized\n");
		return false;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* glfw_window = glfwCreateWindow(window->get_width() ,
												window->get_height(),
												window->getTitle().c_str(), nullptr, nullptr);
	window->set_glfw_window(glfw_window);

	if (!glfw_window)
	{
		printf("Error: GLFW Window couldn't be created\n");
		return false;
	}

	glfwSetWindowUserPointer(glfw_window, window);
	glfwSetWindowSizeCallback(glfw_window, on_window_size_callback);
	glfwSetWindowCloseCallback(glfw_window, on_window_close_callback);
	//inputManager
	glfwSetCursorPosCallback(glfw_window, on_cursor_callback);
	glfwSetKeyCallback(glfw_window, on_key_callback);
	glfwSetMouseButtonCallback(glfw_window, on_mouse_callback);

	glfwMakeContextCurrent(glfw_window);
	glfwSwapInterval(1);
	/* Initialize GLEW */

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{		
		printf("Error: %s\n", glewGetErrorString(err));
		return false;
	}
	glEnable(GL_DEPTH_TEST);
	return true;
}

void GLContext::pre_render() {
	glViewport(0, 0, window->get_width(), window->get_height());
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLContext::post_render() {
	glfwPollEvents();
	glfwSwapBuffers(window->get_glfw_window());
}

void GLContext::clear() {
	glfwDestroyWindow(window->get_glfw_window());
	glfwTerminate();
}


void GLContext::grab_mouse(bool param) {
	if (param) {
		glfwSetInputMode(window->get_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		cursor_visible = true;
	}
	else {
		glfwSetInputMode(window->get_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		cursor_visible = false;
	}

}