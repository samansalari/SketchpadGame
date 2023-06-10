#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

class Texture2D
{
public:
    
    unsigned int _id;    
    unsigned int _width, _height; 

    unsigned int _internalFormat;
    unsigned int _imageFormat;
    
    // texture configuration
    unsigned int _wrap_s; 
    unsigned int _wrap_t; 
    unsigned int _filter_min; 
    unsigned int _filter_max; 
    
    Texture2D();    
    void generate(unsigned int width, unsigned int height, unsigned char* data);    
    void bind() const;
};

#endif