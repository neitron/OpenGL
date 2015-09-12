#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once

#include <string>

#include <GL\glew.h>
#include <SOIL.h>

// Класс текстуры
class Texture
{
public:

  Texture(GLenum textureTarget, const std::string &fileName);
  ~Texture();
  
  // Загрузка текстуры
  bool Load();
  // Задает активный объект текстуры
  void Bind(GLenum textureUnit);
    
private:

  std::string m_fileName; // Имя файла текстуры
  GLenum m_textureTarget; // Указатель на текстуру
  GLuint m_textureObject; // Массив объектов текстур
  unsigned char* m_image; // Загруженное изображение
  GLint m_Width;    // Запомним ширину изображения
  GLint m_Height;   // Высота
  /*
  Magick::Image *m_pImage;
  Magick::Blob m_blob;*/
};

#endif /*TEXTURE*/