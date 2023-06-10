#include "Voxel.h"

Voxel::Voxel():enable(true){
	 
}

void Voxel::init(glm::vec4 colour, glm::vec3 position) {
	this->colour = colour;
	this->position = position;
	mass = 1;
}