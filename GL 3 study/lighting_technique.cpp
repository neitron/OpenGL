#pragma once

#include <iostream>
#include <fstream>

#include "lighting_technique.h"
#include "utils.h"

LightingTechnique::LightingTechnique ( )
{ 
}


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
  m_WorldMatrixLocation = GetUniformLocation ( "gWorld" );
  m_samplerLocation     = GetUniformLocation ( "gSampler" );

  m_dirLightLocation.color            =   GetUniformLocation ( "gDirectionalLight.base.color" );
  m_dirLightLocation.ambientIntensity =   GetUniformLocation ( "gDirectionalLight.base.ambientIntensity" );
  m_dirLightLocation.diffuseIntensity =   GetUniformLocation ( "gDirectionalLight.base.diffuseIntensity" );
  m_dirLightLocation.direction        =   GetUniformLocation ( "gDirectionalLight.direction" );

  m_eyeWorldPosLocation           = GetUniformLocation ( "gEyeWorldPos" );
  m_matSpecularIntensityLocation  = GetUniformLocation ( "gMatSpecularIntensity" );
  m_matSpecularPowerLocation      = GetUniformLocation ( "gSpecularPower" );

  m_numPointLightsLocation = GetUniformLocation ( "gNumPointLights" );


  for ( unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS ( m_pointLightsLocation ); i++ ) 
  {
    char name[128] = { 0 };

    _snprintf_s ( name, sizeof ( name ), "gPointLights[%d].base.color", i );
    m_pointLightsLocation[i].color = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gPointLights[%d].base.ambientIntensity", i );
    m_pointLightsLocation[i].ambientIntensity = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gPointLights[%d].base.diffuseIntensity", i );
    m_pointLightsLocation[i].diffuseIntensity = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gPointLights[%d].position", i );
    m_pointLightsLocation[i].position = GetUniformLocation ( name );

    

    _snprintf_s ( name, sizeof ( name ), "gPointLights[%d].atten.constant", i );
    m_pointLightsLocation[i].atten.constant = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gPointLights[%d].atten.linear", i );
    m_pointLightsLocation[i].atten.linear = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gPointLights[%d].atten.expVar", i );
    m_pointLightsLocation[i].atten.exp = GetUniformLocation ( name );

    if (
      m_pointLightsLocation[i].color            == INVALID_UNIFORM_LOCATION ||
      m_pointLightsLocation[i].ambientIntensity == INVALID_UNIFORM_LOCATION ||
      m_pointLightsLocation[i].position         == INVALID_UNIFORM_LOCATION ||
      m_pointLightsLocation[i].diffuseIntensity == INVALID_UNIFORM_LOCATION ||
      m_pointLightsLocation[i].atten.constant   == INVALID_UNIFORM_LOCATION ||
      m_pointLightsLocation[i].atten.linear     == INVALID_UNIFORM_LOCATION ||
      m_pointLightsLocation[i].atten.exp        == INVALID_UNIFORM_LOCATION 
      ) 
    {
      return false;
    }
  }


  if ( 
    m_WVPLocation                         == INVALID_UNIFORM_LOCATION ||
    m_WorldMatrixLocation                 == INVALID_UNIFORM_LOCATION ||
    m_samplerLocation                     == INVALID_UNIFORM_LOCATION ||
    m_dirLightLocation.ambientIntensity   == INVALID_UNIFORM_LOCATION ||
    m_dirLightLocation.color              == INVALID_UNIFORM_LOCATION ||
    m_dirLightLocation.diffuseIntensity   == INVALID_UNIFORM_LOCATION ||
    m_dirLightLocation.direction          == INVALID_UNIFORM_LOCATION ||
    m_eyeWorldPosLocation                 == INVALID_UNIFORM_LOCATION ||
    m_matSpecularIntensityLocation        == INVALID_UNIFORM_LOCATION ||
    m_matSpecularPowerLocation            == INVALID_UNIFORM_LOCATION ||
    m_numPointLightsLocation              == INVALID_UNIFORM_LOCATION 
    )
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

void LightingTechnique::SetPointLights ( unsigned int numLights, const PointLight* pLights )
{
  glUniform1i ( m_numPointLightsLocation, numLights );

  for ( unsigned int i = 0; i < numLights; i++ ) 
  {
    glUniform3f ( m_pointLightsLocation[i].color,             pLights[i].color.x, pLights[i].color.y, pLights[i].color.z );
    glUniform1f ( m_pointLightsLocation[i].ambientIntensity,  pLights[i].ambientIntensity );
    glUniform1f ( m_pointLightsLocation[i].diffuseIntensity,  pLights[i].diffuseIntensity );
    glUniform3f ( m_pointLightsLocation[i].position,          pLights[i].position.x, pLights[i].position.y, pLights[i].position.z );
    glUniform1f ( m_pointLightsLocation[i].atten.constant,    pLights[i].attenuation.constant );
    glUniform1f ( m_pointLightsLocation[i].atten.linear,      pLights[i].attenuation.linear );
    glUniform1f ( m_pointLightsLocation[i].atten.exp,         pLights[i].attenuation.exp );
  }
}