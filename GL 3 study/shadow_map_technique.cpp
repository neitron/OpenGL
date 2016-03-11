#include "shadow_map_technique.h"
#include "utils.h"

ShadowMapTechnique::ShadowMapTechnique ( )
{
}

bool ShadowMapTechnique::Init ( )
{
  if ( !Technique::Init ( ) ) 
  {
    return false;
  }

  bool  succes = true;

  char* pShaderText = nullptr;

  succes =
    LoadShaderTextFile ( "VS_shadow_map.glsl", pShaderText ) &&
    AddShader ( GL_VERTEX_SHADER, pShaderText );

  if ( !succes ) 
  {
    return false;
  }

  succes =
    LoadShaderTextFile ( "FS_shadow_map.glsl", pShaderText ) &&
    AddShader ( GL_FRAGMENT_SHADER, pShaderText );

  if ( !succes ) 
  {
    return false;
  }

  if ( !Finalize ( ) ) 
  {
    return false;
  }

  m_WVPLocation = GetUniformLocation ( "gWVP" );
  m_textureLocation = GetUniformLocation ( "gShadowMap" );

  if ( 
    m_WVPLocation     == INVALID_UNIFORM_LOCATION ||
    m_textureLocation == INVALID_UNIFORM_LOCATION 
    ) 
  {
    return false;
  }

  return true;
}

void ShadowMapTechnique::SetWVP ( const Matrix4f& WVP )
{
  glUniformMatrix4fv ( m_WVPLocation, 1, GL_TRUE, ( const GLfloat* ) WVP.m );
}

void ShadowMapTechnique::SetTextureUnit ( unsigned int textureUnit )
{
  glUniform1i ( m_textureLocation, textureUnit );
}