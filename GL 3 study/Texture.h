#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once

#include <string>

#include <GL\glew.h>
#include <SOIL.h>


class Texture
{
public:

  Texture(GLenum textureTarget, const std::string &fileName);
  ~Texture() 
  { 
    //SOIL_free_image_data(m_image); 
    glBindTexture(m_textureTarget, 0);
  } 
  bool Load();

  void Bind(GLenum textureUnit);
    
private:
  std::string m_fileName;
  GLenum m_textureTarget;
  GLuint m_textureObject;
  unsigned char* m_image;
  GLint m_Width;
  GLint m_Height;/*
  Magick::Image *m_pImage;
  Magick::Blob m_blob;*/
};

#endif /*TEXTURE*/