#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once
#include <GL\glut.h>
#include <string>
#include <ImageMagick\Magick++.h>

class Texture
{
public:
  
    Texture(GLenum TextureTarget, const std::string& FileName)
    {
      Magick::Image* i = 0;
    }
    
    //bool Load();
 
    //void Bind(GLenum TextureUnit);
};

#endif /*TEXTURE*/