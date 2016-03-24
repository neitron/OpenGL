#pragma once

#include "billboard_technique.h"
#include "utils.h"

BillboardTechnique::BillboardTechnique ( )
{ }

bool BillboardTechnique::Init ( )
{
  if ( !Technique::Init ( ) ) 
  {
    return false;
  }

  if ( 
    !LoadShader ( GL_VERTEX_SHADER, "VS_billboards.glsl" ) ||
    !LoadShader ( GL_GEOMETRY_SHADER, "GS_billboards.glsl" ) ||
    !LoadShader ( GL_FRAGMENT_SHADER, "FS_billboards.glsl" ) )
  {
    return false;
  }

  if ( !Finalize ( ) ) 
  {
    return false;
  }

  m_VPLocation        = GetUniformLocation ( "gVP" );
  m_cameraPosLocation = GetUniformLocation ( "gCameraPos" );
  m_colorMapLocation  = GetUniformLocation ( "gColorMap" );

  if ( 
    m_VPLocation        == INVALID_UNIFORM_LOCATION ||
    m_cameraPosLocation == INVALID_UNIFORM_LOCATION ||
    m_colorMapLocation  == INVALID_UNIFORM_LOCATION ) 
  {
    return false;
  }

  return true;
}

void BillboardTechnique::SetVP ( const Matrix4f & VP )
{
  glUniformMatrix4fv ( m_VPLocation, 1, GL_TRUE, ( const GLfloat* ) VP.m );
}

void BillboardTechnique::SetCameraPosition ( const Vector3f & pos )
{
  glUniform3f ( m_cameraPosLocation, pos.x, pos.y, pos.z );
}

void BillboardTechnique::SetColorTextureUnit ( unsigned int textureUnit )
{
  glUniform1ui ( m_colorMapLocation, textureUnit );
}