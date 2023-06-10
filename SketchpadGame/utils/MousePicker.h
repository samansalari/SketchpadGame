#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Window;
class Camera;

class MousePicker {
public:
	MousePicker(Window* window_p, glm::mat4 projectionMatrix_p);

	void update();

	

	glm::vec3 getCurrentRay() const { return currentRay; }
	glm::vec3 getPointOnRay(float distance) const;
private:
	glm::vec3 calculateMouseRay();
	glm::vec2 getNormalizedDeviceCoords(float mouseX, float mouseY) const;
	glm::vec4 toEyeCoords(glm::vec4 clipCoords) const;
	glm::vec3 toWorldCoords(glm::vec4 eyeCoords) const;	

private:
	glm::vec3 currentRay;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	Window* window;
	Camera* camera;


};