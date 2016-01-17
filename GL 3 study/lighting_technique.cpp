#pragma once

#include <iostream>
#include <fstream>

#include "lighting_technique.h"


//static const char* pVS = "";
//static const char* pFS = "";


LightingTechnique::LightingTechnique ( )
{ }


bool LightingTechnique::SetShader ( const char* pFilename, char* &pShaderText )
{
  std::ifstream is ( pFilename, std::ios::in | std::ios::binary | std::ios::ate );

  if ( !is.is_open ( ) )
  {
    std::cerr << "Unable to open file " << pFilename << std::endl;
    return false;
  }

  long size = is.tellg ( );
  pShaderText = new char[size + 1];

  is.seekg ( 0, std::ios::beg );
  is.read ( pShaderText, size );
  is.close ( );

  pShaderText[size] = 0;

  std::cout << pShaderText << std::endl << std::endl;

  return true;
}


bool LightingTechnique::Init ( )
{
  if ( !Technique::Init ( ) )
  {
    return false;
  }

  char* pVS = nullptr;
  SetShader ( "VS.glsl", pVS );

  char* pFS = nullptr;
  SetShader ( "FS.glsl", pFS );

  if ( !AddShader ( GL_VERTEX_SHADER, pVS ) )
  {
    return false;
  }

  if ( !AddShader ( GL_FRAGMENT_SHADER, pFS ) )
  {
    return false;
  }

  if ( !Finalize ( ) )
  {
    return false;
  }

  m_WVPLocation = GetUniformLocation ( "gWVP" );
  m_samplerLocation = GetUniformLocation ( "gSampler" );
  m_dirLightColorLocation = GetUniformLocation ( "gDirectionalLight.color" );
  m_dirLightAmbientIntensityLocation = GetUniformLocation ( "gDirectionalLight.ambientIntensity" );

  if ( m_dirLightAmbientIntensityLocation == 0xFFFFFFFF ||
    m_WVPLocation == 0xFFFFFFFF ||
    m_samplerLocation == 0xFFFFFFFF ||
    m_dirLightColorLocation == 0xFFFFFFFF )
  {
    return false;
  }

  return true;
}


void LightingTechnique::SetWVP ( const Matrix4f* WVP )
{
  glUniformMatrix4fv ( m_WVPLocation, 1, GL_TRUE, reinterpret_cast< const GLfloat* >( WVP->m ) );
}


void LightingTechnique::SetTextureUnit ( unsigned int textureUnit )
{
  glUniform1i ( m_samplerLocation, textureUnit );
}


void LightingTechnique::SetDirectionalLight ( const DirectionLight& light )
{
  glUniform3f ( m_dirLightColorLocation, light.color.x, light.color.y, light.color.z );
  glUniform1f ( m_dirLightAmbientIntensityLocation, light.ambientIntensity );
}