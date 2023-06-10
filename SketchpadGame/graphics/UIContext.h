#pragma once

class Window;

class UIContext {
public:
	UIContext(Window* win);
	~UIContext();

	bool init();

	void pre_render();

	void post_render();

	void clear();

private:
	Window* window;
};