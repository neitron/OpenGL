#ifndef SKYBOXTECHNIQUE_H
#define SKYBOXTECHNIQUE_H

#pragma once

#include "math3d.h"
#include "technique.h"

class SkyboxTechnique : public Technique
{
public:

  SkyboxTechnique ( );

  virtual bool Init ( );

  void SetWVP ( const Matrix4f& WVP );
  void SetTextureUnit ( unsigned int TextureUnit );

private:

  GLuint m_WVPLocation;
  GLuint m_textureLocation;
};

#endif /* SKYBOXTECHNIQUE_H */
