#pragma once

class MainPanel {
public:
	MainPanel(Window* win);
	void render();
private:
	bool buttonPCEnabled = true; //PC for PeerConnect

	Window* window;
};