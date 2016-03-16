#ifndef LIGHTINGTECHNIQUE_H
#define LIGHTINGTECHNIQUE_H

#pragma once

#include "technique.h"
#include "math3d.h"

const unsigned int MAX_POINT_LIGHTS = 2u; // Максимальное количество источников точесного света
const unsigned int MAX_SPOT_LIGHTS  = 2u; // Максимальное количество источников прожекторного света


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Base light for diferent sourse
struct BaseLight
{
  Vector3f  color;            // Цвет света
  float     ambientIntensity; // Интенсивность вонового света (генерального)
  float     diffuseIntensity; // Интенсивность скользящего света (от источника)

  BaseLight ( ) :
    color             ( Vector3f ( 0.0f, 0.0f, 0.0f ) ),
    ambientIntensity  (0.0f),
    diffuseIntensity  (0.0f)
  { }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Direction light
struct DirectionLight : BaseLight
{
  Vector3f  direction;  // направление освещения (от направленого света)

  DirectionLight ( ) :
    direction ( Vector3f ( 0.0f, 0.0f, 0.0f ) )
  { }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Point light
struct PointLight : BaseLight
{
  Vector3f  position;

  // Параметры затухания для более гибкого управления точечным светом
  struct
  {
    float constant;
    float linear;
    float exp;
  } attenuation; // Затухание

  PointLight ( ):
    position ( Vector3f ( 0.0f, 0.0f, 0.0f ) )
  {
    attenuation.constant = 1.0f;
    attenuation.linear = 0.0f;
    attenuation.exp = 0.0f;
  }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Spot light
struct SpotLight : public PointLight
{
  Vector3f  direction;
  float     cutOff;    // Область отсечения, максимальный угол между направлением света и вектором до пикселей, которые еще попадут под влияние света

  SpotLight ( ) :
    direction   ( Vector3f ( 0.0f, 0.0f, 0.0f ) ),
    cutOff      (0.0f)
  { }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LightingTechnique : public Technique
{
public:

  LightingTechnique ( );
  virtual bool Init ( );

  // Матрица преобразования (world view projection)
  void SetWVP ( const Matrix4f& WVP );
  // Мировая матрица преобразования
  void SetWorldMatrix ( const Matrix4f& WorldInverse );
  // Указывает единицу текстуры в сэмплере
  void SetTextureUnit ( unsigned int textureUnit ); 

  // Направленное освещение:
  // Устанавливает направленное освещение
  void SetDirectionalLight ( const DirectionLight& light );

  // Отраженный свет:
  // Устанавливает в шейдере позицию зрителя
  void SetEyeWorldPos           ( const Vector3f& eyeWorldPos );  
  // Устанавливает в шейдере интенсивность отраженного света
  void SetMatSpecularIntensity  ( float intensity );              
  // Устанавливает в шейдере степень отражения материала
  void SetMatSpecularPower      ( float power );                  

  // Точечный свет:
  // Устанавливает несколько источников света
  void SetPointLights ( unsigned int numLights, const PointLight* pLights );

  // Прожекторный свет:
  // Устанавливает несколько источников света
  void SetSpotLights ( unsigned int numLights, const SpotLight* pLights );
  
  // Матрица преобразования из позиции источника света (world view projection)
  void SetLightWVP ( const Matrix4f& lightWVP );
  // Указывает единицу текстуры в сэмплере для карты теней
  void SetShadowMapTextureUnit ( unsigned int textureUnit );

private:

  // Расположения Юниформ (далее Ю) в шейдерах:
  // 1 Матриц преобразований:

  GLuint m_WVPLocation;
  GLuint m_WorldMatrixLocation;

  GLuint m_samplerLocation; // Ю-позиция семплера текстуры
                               
  // 2 Отраженного света:

  GLuint m_eyeWorldPosLocation;           // Ю-позиция зритель
  GLuint m_matSpecularIntensityLocation;  // Ю-позиция интенсивности отражения
  GLuint m_matSpecularPowerLocation;      // Ю-позиция степени отражения материала

  // 3 Направленного света
  struct 
  {
    GLuint color;
    GLuint ambientIntensity;
    GLuint direction;
    GLuint diffuseIntensity;

  } m_dirLightLocation; // Ю-позиция направленного источника света

  // 4 Точечного света
  struct 
  {
    GLuint color;
    GLuint ambientIntensity;
    GLuint diffuseIntensity;
    GLuint position;

    struct
    {
      GLuint constant;
      GLuint linear;
      GLuint exp;
    } atten;

  } m_pointLightsLocation[MAX_POINT_LIGHTS]; // Массив Ю-позиций источников точечного света

  GLuint m_numPointLightsLocation; // Ю-позиция количество точечных источников света

  // 5 Прожекторного света
  struct 
  {
    GLuint color;
    GLuint ambientIntensity;
    GLuint diffuseIntensity;
    GLuint position;
    GLuint direction;
    GLuint cutOff;
    
    struct
    {
      GLuint constant;
      GLuint linear;
      GLuint exp;
    } atten;

  } m_spotLightsLocation[MAX_SPOT_LIGHTS]; // Массив Ю-позиций источников прожекторного света

  GLuint m_numSpotLightsLocation; // Ю-позиция количество прожекторных источников света

  // 6 Для карты теней
  GLuint m_LightWVPLocation;	// Ю-позиция мaрицы преобразования с позиции света
  GLuint m_shadowMapLocation;	// Ю-позиция модуля текстуры для карты теней

};

#endif /* LIGHTINGTECHNIQUE_H */