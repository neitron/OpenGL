#pragma once

#include <iostream>
#include <fstream>

#include "lighting_technique.h"


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

  long size = (long) is.tellg ( );
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

  m_WVPLocation         = GetUniformLocation ( "gWVP" );
  m_samplerLocation     = GetUniformLocation ( "gSampler" );
  m_WorldMatrixLocation = GetUniformLocation ( "gWorld" );

  m_dirLightLocation.color            =   GetUniformLocation ( "gDirectionalLight.color" );
  m_dirLightLocation.ambientIntensity =   GetUniformLocation ( "gDirectionalLight.ambientIntensity" );
  m_dirLightLocation.diffuseIntensity =   GetUniformLocation ( "gDirectionalLight.diffuseIntensity" );
  m_dirLightLocation.direction        =   GetUniformLocation ( "gDirectionalLight.direction" );

  m_eyeWorldPosLocation           = GetUniformLocation ( "gEyeWorldPos" );
  m_matSpecularIntensityLocation  = GetUniformLocation ( "gMatSpecularIntensity" );
  m_matSpecularPowerLocation      = GetUniformLocation ( "gSpecularPower" );

  if ( 
    m_WVPLocation == 0xFFFFFFFF ||
    m_WorldMatrixLocation == 0xFFFFFFFF ||
    m_samplerLocation == 0xFFFFFFFF ||
    m_dirLightLocation.ambientIntensity == 0xFFFFFFFF ||
    m_dirLightLocation.color == 0xFFFFFFFF ||
    m_dirLightLocation.diffuseIntensity == 0xFFFFFFFF ||
    m_dirLightLocation.direction == 0xFFFFFFFF ||
    m_eyeWorldPosLocation == 0xFFFFFFFF ||
    m_matSpecularIntensityLocation == 0xFFFFFFFF ||
    m_matSpecularPowerLocation == 0xFFFFFFFF )
  {
    return false;
  }

  return true;
}


void LightingTechnique::SetWVP ( const Matrix4f& WVP )
{
  glUniformMatrix4fv ( m_WVPLocation, 1, GL_TRUE, reinterpret_cast< const GLfloat* >( WVP.m ) );
}

void LightingTechnique::SetWorldMatrix ( const Matrix4f& WorldInverse )
{
  glUniformMatrix4fv ( m_WorldMatrixLocation, 1, GL_TRUE, ( const GLfloat* ) WorldInverse.m );
}

void LightingTechnique::SetTextureUnit ( unsigned int textureUnit )
{
  glUniform1i ( m_samplerLocation, textureUnit );
}

void LightingTechnique::SetMatSpecularIntensity ( float intensity )
{
  glUniform1f ( m_matSpecularIntensityLocation, intensity );
}

void LightingTechnique::SetMatSpecularPower ( float power )
{
  glUniform1f ( m_matSpecularPowerLocation, power );
}

void LightingTechnique::SetEyeWorldPos ( const Vector3f& eyeWorldPos )
{
  glUniform3f ( m_eyeWorldPosLocation, eyeWorldPos.x, eyeWorldPos.y, eyeWorldPos.z );
}

void LightingTechnique::SetDirectionalLight ( const DirectionLight& light )
{
  glUniform3f ( m_dirLightLocation.color, light.color.x, light.color.y, light.color.z );
  glUniform1f ( m_dirLightLocation.ambientIntensity, light.ambientIntensity );

  Vector3f direction = light.direction;
  direction.Normalize ( );
  glUniform3f ( m_dirLightLocation.direction, direction.x, direction.y, direction.z );
  glUniform1f ( m_dirLightLocation.diffuseIntensity, light.diffuseIntensity );
}