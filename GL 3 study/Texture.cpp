#pragma once

#include <iostream>

#include <SOIL.h>

#include "Texture.h"


Texture::Texture(GLenum textureTarget, const std::string &fileName)
{
  m_textureTarget = textureTarget;
  m_fileName      = fileName;
  m_image         = nullptr;
}

Texture::~Texture() 
{ 
  SOIL_free_image_data(m_image);      // ������������ ������ ��������
  glBindTexture(m_textureTarget, 0);  // ���������� �� ������ � �������
} 

// �������� ��������
bool Texture::Load()
{
  m_image = SOIL_load_image(m_fileName.c_str(), &m_Width, &m_Height, 0, SOIL_LOAD_RGBA);
  
  #pragma region If we use ImageMagic.lib
  /*try
  m_pImage = new Magick::Image(m_fileName);
  m_pImage->write(&m_blob, "RGBA");
  }
  catch (Magick::Error& Error) {
  std::cout << "Error loading texture '" << m_fileName << "': " << Error.what() << std::endl;
  return false;
  }*/
  #pragma endregion

  // ���������� ������� ���������� �������� ������� � �������� �� � ������
  glGenTextures(1, &m_textureObject);

  // �������� ������ ��������, ������ ���� ������ ��������� �� ���� �������
  // 1: ������� (GL_TEXTURE_2D)
  glBindTexture(m_textureTarget, m_textureObject);

  // ������� ��� �������� ������� ����� ������� ��������, ��� �� ����, ���� ������ ��������
  glTexImage2D(m_textureTarget, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image);
  
  // ����� �� ��������� �������, ������� ����� ������������ ��� ���������� � �������������
  glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return true;
}

// ������ �������� ������ ��������
void Texture::Bind(GLenum textureUnit)
{
  glActiveTexture ( textureUnit );

  // �������� ������ ��������, ������ ���� ������ ��������� �� ���� �������
  // 1: ������� (GL_TEXTURE_2D)
  glBindTexture ( m_textureTarget, m_textureObject );
}