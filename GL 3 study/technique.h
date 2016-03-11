#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#pragma once

#include <list>

#include <GL\glew.h>


class Technique
{
public:
  
  Technique ( );
  ~Technique ( );

  virtual bool Init ( );

  void Enable ( );

protected:

  bool LoadShaderTextFile ( const char* pFilename, char* &pShaderText );
  bool AddShader ( GLenum shaderType, const char* pShaderText );
  bool Finalize ( );

  GLint GetUniformLocation ( const char* pUniformName );

private:

  GLuint m_shaderProgram;

  typedef std::list<GLuint> ShaderObjectList;
  ShaderObjectList m_shaderObjectList;

};


#endif /* TECHNIQUE_H */
