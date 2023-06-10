#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Voxel {
public:
	Voxel();
	void init(glm::vec4 colour, glm::vec3 position);

	void setEnable(bool param) { enable = param; }
	bool isEnabled() const { return enable; }

	glm::vec3 getPosition() const { return position; }

	glm::vec4 getColour() const { return colour; }

	void setColour(glm::vec4 colour) { this->colour = colour; }

	void reduceMass() {
		mass--;
		if (mass <= 0) {
			mass = 0;
			setEnable(false);
		}
	}

	void increaseMass(glm::vec3 colour_p /*colour of the stolen voxel*/) {
		mass++;

		if (enable) {
			colour.x = (colour.x + colour_p.x) / 2.0f;
			colour.y = (colour.y + colour_p.y) / 2.0f;
			colour.z = (colour.z + colour_p.z) / 2.0f;
		}
		else {
			colour = glm::vec4(colour_p.x, colour_p.y, colour_p.z, 1.0f);
			setEnable(true);
		}		
	}

private:
	bool enable;
	int mass;
	glm::vec4 colour;
	glm::vec3 position;
};