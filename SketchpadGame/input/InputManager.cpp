#include "core/Window.h"
#include "InputManager.h"


bool firstMouse_ = true;
float lastX_ = 0.0f;
float lastY_ = 0.0f;
void InputManager::handleCursorPositionEvent(double xpos, double ypos)
{
    mouseX = xpos;
    mouseY = ypos;

	if (window->getGLContext()->isCursorVisible()) {
		firstMouse_ = true;
		return;
	}
	if (firstMouse_)
	{
		lastX_ = (float)xpos;
		lastY_ = (float)ypos;
		firstMouse_ = false;
	}

	float xoffset = (float)xpos - lastX_;
	float yoffset = lastY_ - (float)ypos;

	lastX_ = (float)xpos;
	lastY_ = (float)ypos;

	window->getScene()->getCamera()->ProcessMouseMovement(xoffset, yoffset);
}