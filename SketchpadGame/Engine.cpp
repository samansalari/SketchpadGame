#include "engine.h"


Engine::Engine() {
	_name = "Sketchpad Game";
	_window = std::make_unique<Window>();
	_window->init(840, 640, _name);

	_timer = std::make_unique<Timer>();
	_timer->Start();
}


Engine& Engine::getInstance() {
	static Engine engine;
	return engine;
}


void Engine::run() {
	
	while (_window->is_running())
	{		
		float dt = static_cast<float>(_timer->GetMilisecondsElapsed() / 1000.0f);
		
		_window->frame(dt);
		
		_timer->Restart();
	}
	_window->clear();
}

