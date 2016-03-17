#ifndef CUBEMAP_H
#define CUBEMAP_H

#pragma once

#include <string>

#include <GL/glew.h>

using std::string;

class CubemapTexture
{
public:
  
  CubemapTexture ( const string& Directory,
    const string& PosXFilename,
    const string& NegXFilename,
    const string& PosYFilename,
    const string& NegYFilename,
    const string& PosZFilename,
    const string& NegZFilename );

  ~CubemapTexture ( );

  bool Load ( );

  void Bind ( GLenum TextureUnit );

private:

  struct
  {
    string fileName;
    GLint  width;  // Запомним ширину изображения
    GLint  height; // И высотy
  } m_images[6];

  GLuint m_textureObj;
};

#endif /* CUBEMAP_H */

