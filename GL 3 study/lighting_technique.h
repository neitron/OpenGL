#ifndef LIGHTINGTECHNIQUE_H
#define LIGHTINGTECHNIQUE_H

#pragma once

#include "technique.h"
#include "math3d.h"

struct DirectionLight
{
  Vector3f  color;
  float     ambientIntensity;
  Vector3f  direction;
  float     diffuseIntensity;
};

class LightingTechnique : public Technique
{
public:

  LightingTechnique ( );
  virtual bool Init ( );

  bool SetShader ( const char* pFilename, char* &pShaderText );

  void SetWVP ( const Matrix4f& WVP );
  void SetWorldMatrix ( const Matrix4f& WorldInverse );
  // Указывает единицу текстуры в сэмплере
  void SetTextureUnit ( unsigned int textureUnit );         
  
  // Фоновое освещение
  void SetDirectionalLight ( const DirectionLight& light );

  // Отраженный свет
  void SetEyeWorldPos ( const Vector3f& eyeWorldPos );  // зритель
  void SetMatSpecularIntensity ( float intensity );     // интенсивность отражения
  void SetMatSpecularPower ( float power );             // степень отражения материала

private:

  GLuint m_WVPLocation;
  GLuint m_WorldMatrixLocation;

  GLuint m_samplerLocation;
                                                
  GLuint m_eyeWorldPosLocation;           // зритель
  GLuint m_matSpecularIntensityLocation;  // интенсивность отражения
  GLuint m_matSpecularPowerLocation;      // степень отражения материала
                                          
  struct 
  {
    GLuint color;
    GLuint ambientIntensity;
    GLuint direction;
    GLuint diffuseIntensity;
  } m_dirLightLocation;


};

#endif // LIGHTINGTECHNIQUE_H