#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once

#include <string>

#include <GL\glew.h>
#include <Magick++.h>

enum : GLenum 
{
  q,w
};

class Texture
{
public:
  
  Texture(GLenum textureTarget, const std::string& fileName);

  bool Load();

  void Bind(GLenum textureUnit);
    
private:
  std::string m_fileName;
  GLenum m_textureTarget;
  GLuint m_textureObject;
  Magick::Image *m_pImage;
  Magick::Blob m_blob;
};

#endif /*TEXTURE*/