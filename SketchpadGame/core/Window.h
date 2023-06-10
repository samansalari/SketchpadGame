#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

#include "graphics/GLContext.h"
#include "graphics/UIContext.h"
#include "ui/Scene.h"
#include "ui/MainPanel.h"


class InputManager;

class Window {
public:	
	Window();
	~Window();

	bool init(int width, int height, const std::string& title);
	void clear();
	void frame(float dt);


	void on_resize(int width, int height);
	void on_close();
	

	bool is_running() const;
	int get_width() const;
	int get_height() const;
	const std::string& getTitle() const;
	GLFWwindow* get_glfw_window() const;
	Scene* getScene() const;
	InputManager* getInputManager() const;
	GLContext* getGLContext() const;

	void set_glfw_window(GLFWwindow* glfw_window);	
private:
	void update();
	void render();
	void handle_input();

private:
	GLFWwindow* glfw_window;

	int _width;
	int _height;
	bool _running;
	std::string _title;
	std::shared_ptr<InputManager> _inputManager;
	std::shared_ptr<GLContext> _glCtx;	
	std::shared_ptr<UIContext> _uiCtx;
	std::shared_ptr<Scene> _scene;
	std::shared_ptr<MainPanel> _mainPanel;
	
};