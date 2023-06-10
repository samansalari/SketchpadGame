#pragma once
#include <string>
#include <memory>
#include "core/Window.h"
#include "utils/Timer.h"

/* Singleton Engine Class */
class Engine {
public:	
	static Engine& getInstance();
	void run();
	
private:
	Engine();
	static Engine& instance;
	
	
	std::string _name;
	std::unique_ptr<Window> _window;		
	std::unique_ptr<Timer> _timer;
};