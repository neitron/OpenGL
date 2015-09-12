
#include <iostream>
#include "Texture.h"


Texture::Texture(GLenum textureTarget, const std::string &fileName)
{
  m_textureTarget = textureTarget;
  m_fileName      = fileName;
  m_image        = 0;
}

Texture::~Texture() 
{ 
  SOIL_free_image_data(m_image); // Высвобождаем данные теустуры
  glBindTexture(m_textureTarget, 0); // Открепляем от актива и позиции
} 

// Загрузка текстуры
bool Texture::Load()
{
  m_image = SOIL_load_image(m_fileName.c_str(), &m_Width, &m_Height, 0, SOIL_LOAD_RGBA);
  /*try 
    m_pImage = new Magick::Image(m_fileName);
    m_pImage->write(&m_blob, "RGBA");
  }
  catch (Magick::Error& Error) {
    std::cout << "Error loading texture '" << m_fileName << "': " << Error.what() << std::endl;
    return false;
  }
  */
  // Генерирует заданое количество объектов текстур и помещает их в массив
  glGenTextures(1, &m_textureObject);
  // Активный объект текстуры, теперь этот объект причастен ко всем вызовам
  // 1: позиция (GL_TEXTURE_2D)
  glBindTexture(m_textureTarget, m_textureObject);
  // Функция для загрузки главной части объекта текстуры, что по сути, сами данные текстуры
  glTexImage2D(m_textureTarget, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image);
  // Здесь мы указываем фильтры, которые будут использованы для увеличения и минимализации
  glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return true;
}

// Задает активный объект текстуры
void Texture::Bind(GLenum textureUnit)
{
  glActiveTexture(textureUnit);
  // Активный объект текстуры, теперь этот объект причастен ко всем вызовам
  // 1: позиция (GL_TEXTURE_2D)
  glBindTexture(m_textureTarget, m_textureObject);
}