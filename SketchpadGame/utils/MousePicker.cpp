#include <iostream>

#include "MousePicker.h"
#include "core/Window.h"
#include "input/InputManager.h"
#include "Camera.h"

MousePicker::MousePicker(Window* window_p, glm::mat4 projectionMatrix_p) {
	window = window_p;
	projectionMatrix = projectionMatrix_p;
	camera = window->getScene()->getCamera();
	viewMatrix = camera->GetViewMatrix();	
}

void MousePicker::update() {
	viewMatrix = camera->GetViewMatrix();
	currentRay = calculateMouseRay();
}


glm::vec3 MousePicker::calculateMouseRay() {
	double mx = window->getInputManager()->getMouseX();
	double my = window->getInputManager()->getMouseY();
	glm::vec2 mouseCursor((float)mx, (float)my);

	glm::vec2 normalizedCoords = getNormalizedDeviceCoords(mouseCursor.x, mouseCursor.y);
	glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
	glm::vec4 eyeCoords = toEyeCoords(clipCoords);
	glm::vec3 worldRay = toWorldCoords(eyeCoords);
	return worldRay;
}
glm::vec2 MousePicker::getNormalizedDeviceCoords(float mouseX, float mouseY) const {
	float display_width = (float)window->get_width();
	float display_height = (float)window->get_height();

	float x = 2 * mouseX / display_width -1;
	float y = 2 * mouseY / display_height - 1;
	return glm::vec2(x, -y);
}
glm::vec4 MousePicker::toEyeCoords(glm::vec4 clipCoords) const {
	glm::mat4 invertedProjection = glm::inverse(projectionMatrix);
	glm::vec4 eyeCoords = invertedProjection * clipCoords;
	return glm::vec4(eyeCoords.x, eyeCoords.y, -1.f, 0.f);
}
glm::vec3 MousePicker::toWorldCoords(glm::vec4 eyeCoords) const {
	glm::mat4 invertedView = glm::inverse(this->viewMatrix);
	glm::vec4 rayWorld = invertedView * eyeCoords;
	glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	mouseRay = glm::normalize(mouseRay);
	return mouseRay;
}

glm::vec3 MousePicker::getPointOnRay(float distance) const {
	glm::vec3 start = camera->Position;
	glm::vec3 scaledRay = glm::vec3(currentRay.x * distance, currentRay.y * distance,
		currentRay.z * distance);

	return glm::vec3(start.x + scaledRay.x, start.y + scaledRay.y, start.z + scaledRay.z);
}