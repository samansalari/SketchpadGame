#include <iostream>

#include "texture.h"


Texture2D::Texture2D()
    : _width(0), _height(0), _internalFormat(GL_RGB), _imageFormat(GL_RGB), _wrap_s(GL_REPEAT), _wrap_t(GL_REPEAT), _filter_min(GL_LINEAR), _filter_max(GL_LINEAR)
{
    glGenTextures(1, &this->_id);
}

void Texture2D::generate(unsigned int width, unsigned int height, unsigned char* data)
{
    this->_width = width;
    this->_height = height;
    // create Texture
    glBindTexture(GL_TEXTURE_2D, this->_id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->_internalFormat, width, height, 0, this->_imageFormat, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->_wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->_wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->_filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->_filter_max);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->_id);
}