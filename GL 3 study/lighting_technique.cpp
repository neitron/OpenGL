#pragma once



#include "lighting_technique.h"
#include "utils.h"
#include "math3d.h"

LightingTechnique::LightingTechnique ( )
{ }


bool LightingTechnique::Init ( )
{
  if ( !Technique::Init ( ) )
  {
    return false;
  }

  bool  succes = true;

  char* pShaderText = nullptr;
  
  succes = 
    LoadShaderTextFile ( "VS.glsl", pShaderText ) &&
    AddShader ( GL_VERTEX_SHADER, pShaderText );

  if ( !succes )
  {
    return false;
  }
  
  succes = 
    LoadShaderTextFile ( "FS.glsl", pShaderText ) &&
    AddShader ( GL_FRAGMENT_SHADER, pShaderText );
  
  if ( !succes )
  {
    return false;
  }

  SafeDelete ( pShaderText );

  if ( !Finalize ( ) )
  {
    return false;
  }

  m_WVPLocation         = GetUniformLocation ( "gWVP" );
  m_worldMatrixLocation = GetUniformLocation ( "gWorld" );
  m_colorMapLocation     = GetUniformLocation ( "gColorMap" );

  m_dirLightLocation.color            =   GetUniformLocation ( "gDirectionalLight.base.color" );
  m_dirLightLocation.ambientIntensity =   GetUniformLocation ( "gDirectionalLight.base.ambientIntensity" );
  m_dirLightLocation.diffuseIntensity =   GetUniformLocation ( "gDirectionalLight.base.diffuseIntensity" );
  m_dirLightLocation.direction        =   GetUniformLocation ( "gDirectionalLight.direction" );

  m_eyeWorldPosLocation           = GetUniformLocation ( "gEyeWorldPos" );
  m_matSpecularIntensityLocation  = GetUniformLocation ( "gMatSpecularIntensity" );
  m_matSpecularPowerLocation      = GetUniformLocation ( "gSpecularPower" );

  m_numPointLightsLocation  =  GetUniformLocation ( "gNumPointLights" );
  m_numSpotLightsLocation   =  GetUniformLocation ( "gNumSpotLights" );

  m_lightWVPLocation  = GetUniformLocation ( "gLightWVP" );
  m_shadowMapLocation = GetUniformLocation ( "gShadowMap" );

  m_normalMapLocation = GetUniformLocation ( "gNormalMap" );
  m_isUseNormalMap    = GetUniformLocation ( "gIsUseNormalMap" );

  if (
    m_WVPLocation                       == INVALID_UNIFORM_LOCATION ||
    m_worldMatrixLocation               == INVALID_UNIFORM_LOCATION ||
    m_colorMapLocation                  == INVALID_UNIFORM_LOCATION ||
    m_dirLightLocation.ambientIntensity == INVALID_UNIFORM_LOCATION ||
    m_dirLightLocation.color            == INVALID_UNIFORM_LOCATION ||
    m_dirLightLocation.diffuseIntensity == INVALID_UNIFORM_LOCATION ||
    m_dirLightLocation.direction        == INVALID_UNIFORM_LOCATION ||
    m_eyeWorldPosLocation               == INVALID_UNIFORM_LOCATION ||
    m_matSpecularIntensityLocation      == INVALID_UNIFORM_LOCATION ||
    m_matSpecularPowerLocation          == INVALID_UNIFORM_LOCATION ||
    m_numPointLightsLocation            == INVALID_UNIFORM_LOCATION ||
    m_numSpotLightsLocation             == INVALID_UNIFORM_LOCATION ||
    m_lightWVPLocation                  == INVALID_UNIFORM_LOCATION ||
    m_shadowMapLocation                 == INVALID_UNIFORM_LOCATION ||
    m_isUseNormalMap                    == INVALID_UNIFORM_LOCATION
    )
  {
    return false;
  }

  for ( unsigned int i = 0u; i < Arraylength ( m_pointLightsLocation ); i++ )
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
  

  for ( unsigned int i = 0; i < Arraylength ( m_spotLightsLocation ); i++ )
  {
    char name[128] = { 0 };

    _snprintf_s ( name, sizeof ( name ), "gSpotLights[%d].base.base.color", i );
    m_spotLightsLocation[i].color = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gSpotLights[%d].base.base.ambientIntensity", i );
    m_spotLightsLocation[i].ambientIntensity = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gSpotLights[%d].base.position", i );
    m_spotLightsLocation[i].position = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gSpotLights[%d].direction", i );
    m_spotLightsLocation[i].direction = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gSpotLights[%d].cutOff", i );
    m_spotLightsLocation[i].cutOff = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gSpotLights[%d].base.base.diffuseIntensity", i );
    m_spotLightsLocation[i].diffuseIntensity = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gSpotLights[%d].base.atten.constant", i );
    m_spotLightsLocation[i].atten.constant = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gSpotLights[%d].base.atten.linear", i );
    m_spotLightsLocation[i].atten.linear = GetUniformLocation ( name );

    _snprintf_s ( name, sizeof ( name ), "gSpotLights[%d].base.atten.expVar", i );
    m_spotLightsLocation[i].atten.exp = GetUniformLocation ( name );

    if ( 
      m_spotLightsLocation[i].color             == INVALID_UNIFORM_LOCATION ||
      m_spotLightsLocation[i].ambientIntensity  == INVALID_UNIFORM_LOCATION ||
      m_spotLightsLocation[i].position          == INVALID_UNIFORM_LOCATION ||
      m_spotLightsLocation[i].direction         == INVALID_UNIFORM_LOCATION ||
      m_spotLightsLocation[i].cutOff            == INVALID_UNIFORM_LOCATION ||
      m_spotLightsLocation[i].diffuseIntensity  == INVALID_UNIFORM_LOCATION ||
      m_spotLightsLocation[i].atten.constant    == INVALID_UNIFORM_LOCATION ||
      m_spotLightsLocation[i].atten.linear      == INVALID_UNIFORM_LOCATION ||
      m_spotLightsLocation[i].atten.exp         == INVALID_UNIFORM_LOCATION 
      ) 
    {
      return false;
    }
  }

  return true;
}




void LightingTechnique::SetWVP ( const Matrix4f& WVP )
{
  glUniformMatrix4fv ( m_WVPLocation, 1, GL_TRUE, reinterpret_cast< const GLfloat* >( WVP.m ) );
}

void LightingTechnique::SetWorldMatrix ( const Matrix4f& WorldInverse )
{
  glUniformMatrix4fv ( m_worldMatrixLocation, 1, GL_TRUE, ( const GLfloat* ) WorldInverse.m );
}

void LightingTechnique::SetColorTextureUnit ( unsigned int textureUnit )
{
  glUniform1i ( m_colorMapLocation, textureUnit );
}

void LightingTechnique::SetNormalMapTextureUnit ( unsigned int textureUnit )
{
  glUniform1i ( m_normalMapLocation, textureUnit );
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

void LightingTechnique::SetSpotLights ( unsigned int numLights, const SpotLight* pLights )
{
  glUniform1i ( m_numSpotLightsLocation, numLights );

  for ( unsigned int i = 0; i < numLights; i++ ) 
  {
    glUniform3f ( m_spotLightsLocation[i].color,            pLights[i].color.x, pLights[i].color.y, pLights[i].color.z );
    glUniform1f ( m_spotLightsLocation[i].ambientIntensity, pLights[i].ambientIntensity );
    glUniform1f ( m_spotLightsLocation[i].diffuseIntensity, pLights[i].diffuseIntensity );
    glUniform3f ( m_spotLightsLocation[i].position,         pLights[i].position.x, pLights[i].position.y, pLights[i].position.z );
    
    Vector3f direction = pLights[i].direction;
    direction.Normalize ( );
    
    glUniform3f ( m_spotLightsLocation[i].direction,      direction.x, direction.y, direction.z );
    glUniform1f ( m_spotLightsLocation[i].cutOff,         cosf ( Angle::ToRadian ( pLights[i].cutOff ) ) );
    glUniform1f ( m_spotLightsLocation[i].atten.constant, pLights[i].attenuation.constant );
    glUniform1f ( m_spotLightsLocation[i].atten.linear,   pLights[i].attenuation.linear );
    glUniform1f ( m_spotLightsLocation[i].atten.exp,      pLights[i].attenuation.exp );
  }
}

void LightingTechnique::SetLightWVP( const Matrix4f &lightWVP )
{
  glUniformMatrix4fv ( m_lightWVPLocation, 1, GL_TRUE, reinterpret_cast< const GLfloat* >( lightWVP.m ) );
}

void LightingTechnique::SetShadowMapTextureUnit( unsigned int textureUnit )
{
  glUniform1i ( m_shadowMapLocation, textureUnit );
}

void LightingTechnique::SetUseNormalMap ( bool isUseNormalMap )
{
  glUniform1i ( m_isUseNormalMap, isUseNormalMap );
}
