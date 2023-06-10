#pragma once
#include "utils/Shader.h"

class Camera;

class VoxelRenderer {
public:
	VoxelRenderer(Camera* camera);
	~VoxelRenderer();

	void drawVoxel(glm::vec3 position, glm::vec4 color = glm::vec4(1.0f),
		glm::vec2 size = glm::vec2(1.0f, 1.0f));

private:
	unsigned int vao, vbo;
	void initRenderData();

	Camera* camera;	
};