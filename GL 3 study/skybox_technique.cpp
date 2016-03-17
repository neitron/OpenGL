#pragma once

#include "skybox_technique.h"
#include "utils.h"

SkyboxTechnique::SkyboxTechnique ( )
{
}

bool SkyboxTechnique::Init ( )
{
  if ( !Technique::Init ( ) ) 
  {
    return false;
  }

  bool  succes = true;

  char* pShaderText = nullptr;

  succes =
    LoadShaderTextFile ( "VS_skybox.glsl", pShaderText ) &&
    AddShader ( GL_VERTEX_SHADER, pShaderText );

  if ( !succes )
  {
    return false;
  }

  succes =
    LoadShaderTextFile ( "FS_skybox.glsl", pShaderText ) &&
    AddShader ( GL_FRAGMENT_SHADER, pShaderText );

  if ( !succes )
  {
    return false;
  }

  SafeDelete ( pShaderText );

  if ( !Finalize ( ) ) {
    return false;
  }

  m_WVPLocation     = GetUniformLocation ( "gWVP" );
  m_textureLocation = GetUniformLocation ( "gCubemapTexture" );

  if ( 
    m_WVPLocation     == INVALID_UNIFORM_LOCATION ||
    m_textureLocation == INVALID_UNIFORM_LOCATION ) 
  {
    return false;
  }

  return true;
}

void SkyboxTechnique::SetWVP ( const Matrix4f & WVP )
{
  glUniformMatrix4fv ( m_WVPLocation, 1, GL_TRUE, ( const GLfloat* ) WVP.m );
}

void SkyboxTechnique::SetTextureUnit ( unsigned int TextureUnit )
{
  glUniform1i ( m_textureLocation, TextureUnit );
}
