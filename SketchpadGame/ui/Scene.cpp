#include "Scene.h"
#include "utils/MousePicker.h"
#include "core/window.h"
#include "input/InputManager.h"
#include "network/Peer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>

Scene::Scene(Window* window) : _window(window) {
    
    srand((unsigned)time(NULL));

}



void Scene::init() {
	ResourceManager::loadTexture("../res/textures/yellow.png", false, "image");	
    ResourceManager::loadShader("../res/shaders/shader.vert", "../res/shaders/shader.frag", nullptr, "shader")
        .setInteger("texture1",1);
	//----------------------------------------//
	_camera = std::make_shared<Camera>();
	_camera->init(glm::vec3(5.0f, 5.0f, 3.0f));
	//----------------------------------------//
    _projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f,
        0.1f, 10000.0f);
    _mousePicker = std::make_shared<MousePicker>(_window, _projection);

    //----------------------------------------// 
    _voxelRenderer = std::make_shared<VoxelRenderer>(_camera.get());
    //----------------------------------------//     
}

void Scene::clear() {

}

void Scene::update() {
    if (_peer == nullptr) return;

    _mousePicker->update();       
    if (_window->getInputManager()->isMouseDragging()) {
        glm::vec3 temp = _mousePicker->getPointOnRay(_camera->Position.z);
        for (Voxel& voxel : _voxels) {
            auto pos = voxel.getPosition();
            if (abs(pos.x - temp.x) <= 0.5f && abs(pos.y - temp.y) <= 0.5f) {

                std::string msg = "F:";
                msg += std::to_string(voxel.getPosition().x) + ":";
                msg += std::to_string(voxel.getPosition().y) + ":";
                msg += "0.0:0.0:0.0";
                _peer->send_message_randomly(msg);
            }
        }
    }

    //---------------------------------
    // TODO
    auto rec = _peer->get_message();
    std::string message = rec.body;

    if (message.compare("NULL")) {
        std::stringstream ss(message);
        std::string token;

        char type = ' '; //F or C
        float colR = 0.0f;
        float colG = 0.0f;
        float colB = 0.0f;
        int posX = 0;
        int poxY = 0;

        std::getline(ss, token, ':');
        std::istringstream(token) >> type;

        std::getline(ss, token, ':');
        std::istringstream(token) >> posX;

        std::getline(ss, token, ':');
        std::istringstream(token) >> poxY;

        std::getline(ss, token, ':');
        std::istringstream(token) >> colR;

        std::getline(ss, token, ':');
        std::istringstream(token) >> colG;

        std::getline(ss, token, ':');
        std::istringstream(token) >> colB;

        std::string msg = "C:";
        for (Voxel& voxel : _voxels) {
            auto pos = voxel.getPosition();
            if (abs(pos.x - posX) <= 0.05f && abs(pos.y - poxY) <= 0.05f) {
                switch (type) {
                case 'C':
                    voxel.increaseMass(glm::vec3(colR, colG, colB));
                    break;
                case 'F':
                    voxel.reduceMass();
                    msg += std::to_string(voxel.getPosition().x) + ":";
                    msg += std::to_string(voxel.getPosition().y) + ":";
                    msg += std::to_string(voxel.getColour().x) + ":";
                    msg += std::to_string(voxel.getColour().y) + ":";
                    msg += std::to_string(voxel.getColour().z) + ":";
                    _peer->send_message(rec.sender_name, msg);                        

                    break;
                default:
                    break;
                }
                break;
            }
        }
    }


}

void Scene::render() {    
    auto shader = ResourceManager::getShader("shader").use();
    shader.setMatrix4("projection", _projection);

    for (Voxel voxel : _voxels) {
        if( voxel.isEnabled() )
            _voxelRenderer->drawVoxel(voxel.getPosition(), voxel.getColour());
    }
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

void Scene::setupPeer(std::string id, int port, float* col) {
    _peer = std::make_shared<Peer>(id, port);
    
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            Voxel voxel;
            voxel.init(glm::vec4(col[0], col[1], col[2], col[3]), glm::vec3(i, j, 0.0f));
            _voxels.push_back(voxel);
        }
    }
}

void Scene::createConnection(std::string id,std::string ip, int port, int sock) {
    PeerInfo info = { ip.c_str(), port, sock};
    _peer->create_connection( id, info );
}

void Scene::broadcast(std::string message) {
    _peer->broadcast( message.c_str() );
}