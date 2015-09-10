
#include <iostream>
#include "Texture.h"

Texture::Texture(GLenum textureTarget, const std::string& fileName)
{
  m_textureTarget = textureTarget;
  m_fileName      = fileName;
  m_pImage        = 0;
}

//bool Texture::Load()
//{
//  try {
//    m_pImage = new Magick::Image(m_fileName);
//    m_pImage->write(&m_blob, "RGBA");
//  }
//  catch (Magick::Error& Error) {
//    std::cout << "Error loading texture '" << m_fileName << "': " << Error.what() << std::endl;
//    return false;
//  }
//
//  glGenTextures(1, &m_textureObject);
//  glBindTexture(m_textureTarget, m_textureObject);
//  glTexImage2D(m_textureTarget, 0, GL_RGB, m_pImage->columns(), m_pImage->rows(), -0.5, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
//  glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//  glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//  return true;
//
//}

void Texture::Bind(GLenum textureUnit)
{
  glActiveTexture(textureUnit);
  glBindTexture(m_textureTarget, m_textureObject);
}