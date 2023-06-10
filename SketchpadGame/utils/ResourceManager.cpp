#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::map<std::string, Texture2D>    ResourceManager::_textures;
std::map<std::string, Shader>       ResourceManager::_shaders;


Shader ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    _shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return _shaders[name];
}

Shader ResourceManager::getShader(std::string name)
{
    return _shaders[name];
}

Texture2D ResourceManager::loadTexture(const char* file, bool alpha, std::string name)
{
    _textures[name] = loadTextureFromFile(file, alpha);
    return _textures[name];
}

Texture2D ResourceManager::getTexture(std::string name)
{
    return _textures[name];
}

void ResourceManager::Clear()
{
    for (auto iter : _shaders)
        glDeleteProgram(iter.second._id);
    for (auto iter : _textures)
        glDeleteTextures(1, &iter.second._id);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{    
    Texture2D texture;
    if (alpha)
    {
        texture._internalFormat = GL_RGBA;
        texture._imageFormat = GL_RGBA;
    }
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);    
    texture.generate(width, height, data);    
    stbi_image_free(data);
    return texture;
}