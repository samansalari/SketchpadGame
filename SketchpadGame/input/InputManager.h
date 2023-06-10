#include <GLFW/glfw3.h>

class Window;

class InputManager
{
public:
    InputManager(Window* window): window(window)
    {

    }

    void handleMouseButtonEvent(int button, int action)
    {
        if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST)
        {
            if (action == GLFW_PRESS)
            {
                mouseButtons[button] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                mouseButtons[button] = false;
                mouseDragging = false;
            }
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            mouseDragging = true;
        }
    }
    void handleCursorPositionEvent(double xpos, double ypos);    
    void handleKeyEvent(int key, int action)
    {
        if (key >= 0 && key < GLFW_KEY_LAST)
        {
            if (action == GLFW_PRESS)
            {
                keys[key] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                keys[key] = false;
            }
        }
    }
public:

    bool isMouseButtonPressed(int button) const
    {
        return mouseButtons[button];
    }

    bool isKeyPressed(int key) const
    {
        return keys[key];
    }

    bool isMouseDragging() const
    {
        return mouseDragging;
    }

    double getMouseX() const
    {
        return mouseX;
    }

    double getMouseY() const
    {
        return mouseY;
    }

private:
    bool mouseButtons[GLFW_MOUSE_BUTTON_LAST] = { false };
    bool keys[GLFW_KEY_LAST] = { false };
    bool mouseDragging = false;
    double mouseX = 0.0;
    double mouseY = 0.0;

    Window* window;
};