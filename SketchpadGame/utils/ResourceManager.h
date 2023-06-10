#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <GL/glew.h>

#include "texture.h"
#include "shader.h"

class ResourceManager
{
public:

    static std::map<std::string, Shader>    _shaders;
    static std::map<std::string, Texture2D> _textures;    
    
    static Shader    loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);    
    static Shader    getShader(std::string name);    
    static Texture2D loadTexture(const char* file, bool alpha, std::string name);    
    static Texture2D getTexture(std::string name);    
    static void      Clear();
private:    
    ResourceManager();
    static Shader    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);    
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
};

#endif