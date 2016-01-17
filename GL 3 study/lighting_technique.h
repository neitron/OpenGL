#ifndef LIGHTINGTECHNIQUE_H
#define LIGHTINGTECHNIQUE_H

#pragma once

#include "technique.h"
#include "math3d.h"

struct DirectionLight
{
  Vector3f  color;
  float     ambientIntensity;
};

class LightingTechnique : public Technique
{
public:

  LightingTechnique ( );
  virtual bool Init ( );

  bool SetShader ( const char* pFilename, char* &pShaderText );

  void SetWVP ( const Matrix4f* WVP );
  // ”казывает единицу текстуры в сэмплере
  void SetTextureUnit ( unsigned int textureUnit );         
  void SetDirectionalLight ( const DirectionLight& light );

private:

  GLuint m_WVPLocation;
  GLuint m_samplerLocation;
  GLuint m_dirLightColorLocation;
  GLuint m_dirLightAmbientIntensityLocation;
};

#endif // LIGHTINGTECHNIQUE_H