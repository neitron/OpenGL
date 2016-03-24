#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "technique.h"
#include "utils.h"

Technique::Technique ( ) :
  m_shaderProgram ( 0 )
{};

Technique::~Technique ( )
{
  for ( auto shader : m_shaderObjectList )
  {
    glDeleteShader ( shader );
  }

  if ( m_shaderProgram != 0 )
  {
    glDeleteProgram ( m_shaderProgram );
    m_shaderProgram = 0;
  }
}

bool Technique::Init ( )
{
  m_shaderProgram = glCreateProgram ( );

  if ( m_shaderProgram == 0 ) 
  {
    fprintf ( stderr, "Error creating shader program\n" );
    return false;
  }

  return true;
}

bool Technique::LoadShaderTextFile ( const char* pFilename, char* &pShaderText )
{
  if ( pShaderText )
  {
    SafeDelete ( pShaderText );
  }

  std::ifstream is ( pFilename, std::ios::in | std::ios::binary | std::ios::ate );

  if ( !is.is_open ( ) )
  {
    std::cerr << "Unable to open file " << pFilename << std::endl;
    return false;
  }

  long size = ( long ) is.tellg ( );
  pShaderText = new char[size + 1];

  is.seekg ( 0, std::ios::beg );
  is.read ( pShaderText, size );
  is.close ( );

  pShaderText[size] = 0;

  std::cout << pShaderText << std::endl << std::endl;

  return true;
}

// Добавление шейдера в шейдерную програму
bool Technique::AddShader ( GLenum shaderType, const char* pShaderText )
{
  // Создаем пустой шейдер
  GLuint shaderObj = glCreateShader ( shaderType );

  if ( shaderObj == 0 ) 
  {
    fprintf ( stderr, "Error creating shader type %d\n", shaderType );
    return false;
  }

  m_shaderObjectList.push_back ( shaderObj );

  const GLchar* p[1];
  p[0] = pShaderText;

  // Процес указания исходников
  // Исходник можно разбить на несколько блоков
  GLint Lengths[1];
  Lengths[0] = strlen ( pShaderText );

  // Загружаем текст (код) в шейдер
  //  | 2: cout of bloks
  glShaderSource ( shaderObj, 1, p, Lengths );

  // Compiling shader
  glCompileShader ( shaderObj );

  // Status of compile
  GLint success;
  glGetShaderiv ( shaderObj, GL_COMPILE_STATUS, &success );

  if ( !success ) 
  {
    GLchar infoLog[1024];
    glGetShaderInfoLog ( shaderObj, 1024, nullptr, infoLog );
    fprintf ( stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog );
    return false;
  }

  // program obj + compil obj
  glAttachShader ( m_shaderProgram, shaderObj );

  return true;
}

bool Technique::LoadShader ( GLenum shaderType, const char * pFilename )
{
  char *pShaderText = 0;
  return
    LoadShaderTextFile ( pFilename, pShaderText ) &&
    AddShader ( shaderType, pShaderText );
}

// Компиляция шейдеров
bool Technique::Finalize ( )
{
  GLint success = 0;
  GLchar errorLog[1024] = { 0 };

  // link of shader program
  glLinkProgram ( m_shaderProgram );

  // status of linker
  glGetProgramiv ( m_shaderProgram, GL_LINK_STATUS, &success );

  if ( success == 0 ) 
  {
    glGetProgramInfoLog ( m_shaderProgram, sizeof ( errorLog ), nullptr, errorLog );
    fprintf ( stderr, "Error linking shader program: '%s'\n", errorLog );
    return false;
  }

  // Cможет ли шейдерная програма загрузиться
  glValidateProgram ( m_shaderProgram );
  glGetProgramiv ( m_shaderProgram, GL_VALIDATE_STATUS, &success );

  if ( !success ) 
  {
    glGetProgramInfoLog ( m_shaderProgram, sizeof ( errorLog ), nullptr, errorLog );
    fprintf ( stderr, "Invalid shader program: '%s'\n", errorLog );
    return false;
  }

  for ( auto shader : m_shaderObjectList )
  {
    glDeleteShader ( shader );
  }

  return true;
}

void Technique::Enable ( )
{
  glUseProgram ( m_shaderProgram );
}

// Запрашиваем позицию (index for acces) uniform variable
GLint Technique::GetUniformLocation ( const char* pUniformName )
{
  GLint location = glGetUniformLocation ( m_shaderProgram, pUniformName );

  if ( location == INVALID_UNIFORM_LOCATION )
  {
    fprintf ( stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName );
  }

  return location;
}