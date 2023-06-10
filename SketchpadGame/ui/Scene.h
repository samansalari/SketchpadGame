#pragma once
#include <memory>
#include "utils/Camera.h"
#include "utils/ResourceManager.h"
#include "Voxel.h"
#include "VoxelRenderer.h"

class Window;
class MousePicker;
class Peer;

class Scene {
public:
	Scene(Window* window);

	void init();
	void clear();
	void update();
	void render();

	Camera* getCamera() const { return _camera.get(); }
	Peer* getPeer() const { return _peer.get(); }

public:
	//Networking
	void setupPeer(std::string id, int port, float* col);
	void createConnection(std::string id, std::string ip, int port, int sock);
	void broadcast(std::string message);

private:	
	std::vector<Voxel> _voxels;
	std::shared_ptr<VoxelRenderer> _voxelRenderer;
	
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<MousePicker> _mousePicker;
	std::shared_ptr<Peer> _peer;
	Window* _window;
private:
	glm::mat4 _projection;
};