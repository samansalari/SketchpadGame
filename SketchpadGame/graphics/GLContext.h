#pragma once

class Window;

class GLContext {
public:
	GLContext(Window* win);

	bool init();

	void pre_render();

	void post_render();

	void clear();

	void grab_mouse(bool param);
	bool isCursorVisible() const { return cursor_visible; }
private:	
	bool cursor_visible = true;
	Window* window;
};