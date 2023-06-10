#include "Window.h"
#include "input/InputManager.h"
#include <iostream>


/************************************************************\
* 
*	Window::Window()
* 
\************************************************************/
Window::Window() : glfw_window(nullptr), _running(true) 
{
	
	_glCtx = std::make_shared<GLContext>(this);
	_scene = std::make_shared<Scene>(this);
	_uiCtx = std::make_shared<UIContext>(this);
	_mainPanel = std::make_shared<MainPanel>(this);	
	_inputManager = std::make_shared<InputManager>(this);
}


/************************************************************\
*
*	Window::~Window
*
\************************************************************/
Window::~Window() {	

}

/************************************************************\
*
*	Window::init()
*
\************************************************************/
bool Window::init(int width, int height, const std::string& title) {
	
	this->_width = width;
	this->_height = height;
	this->_title = title;

	_glCtx->init();
	_uiCtx->init();

	_scene->init();

	return true;
}

/************************************************************\
*
*	Window::clear()
*
\************************************************************/
void Window::clear() {
	_glCtx->clear();
	_uiCtx->clear();

	_scene->clear();
}

/************************************************************\
*
*	Window::frame()
*
\************************************************************/
void Window::frame(float dt) {	
	handle_input();
	update();
	render();
}


/************************************************************\
*
*	Window::update()
*
\************************************************************/
void Window::update() {

	_scene->update();
}

/************************************************************\
*
*	Window::render()
*
\************************************************************/
void Window::render() {
		
	_glCtx->pre_render();
	_uiCtx->pre_render();

	_scene->render();
	_mainPanel->render();

	_uiCtx->post_render();
	_glCtx->post_render();
}


/************************************************************\
*
*	Window::handle_input()
*
\************************************************************/
void Window::handle_input() {
	float deltaTime = 0.025f;

	if (glfwGetKey(glfw_window, GLFW_KEY_W) == GLFW_PRESS)
		_scene->getCamera()->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS)
		_scene->getCamera()->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(glfw_window, GLFW_KEY_A) == GLFW_PRESS)
		_scene->getCamera()->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(glfw_window, GLFW_KEY_D) == GLFW_PRESS)
		_scene->getCamera()->ProcessKeyboard(RIGHT, deltaTime);

	//-------------------------------------------------------------
	if(_inputManager->isKeyPressed(GLFW_KEY_ESCAPE))
		_running = false;	
	if (_inputManager->isKeyPressed(GLFW_KEY_N))
		_glCtx->grab_mouse(true);				
	if (_inputManager->isKeyPressed(GLFW_KEY_B))
		_glCtx->grab_mouse(false);
}



/************************************************************\
*
*	Window::is_running()
*
\************************************************************/
bool Window::is_running() const{
	return _running;
}


/************************************************************\
*
*	Window::get_width()
*
\************************************************************/
int Window::get_width() const {
	return _width;
}


/************************************************************\
*
*	Window::get_height()
*
\************************************************************/
int Window::get_height() const {
	return _height;
}


/************************************************************\
*
*	Window::getTitle()
*
\************************************************************/
const std::string& Window::getTitle() const {
	return _title;
}

/************************************************************\
*
*	Window::getScene()
*
\************************************************************/

Scene* Window::getScene() const {
	return _scene.get();
}


/************************************************************\
*
*	Window::getInputManager()
*
\************************************************************/

InputManager* Window::getInputManager() const {
	return _inputManager.get();
}


/************************************************************\
*
*	Window::getGLContext()
*
\************************************************************/

GLContext* Window::getGLContext() const {
	return _glCtx.get();
}

/************************************************************\
*
*	Window::get_glfw_window()
*
\************************************************************/
GLFWwindow* Window::get_glfw_window() const {
	return glfw_window;
}


/************************************************************\
*
*	Window::set_glfw_window()
*
\************************************************************/
void Window::set_glfw_window(GLFWwindow* glfw_window) {
	this->glfw_window = glfw_window;
}

//=============================================================================//
//=============================================================================//
//								Callbacks
//=============================================================================//

void Window::on_resize(int width, int height)
{
	this->_width = width;
	this->_height = height;

	render();
}

void Window::on_close()
{
	_running = false;
}



