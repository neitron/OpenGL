#ifndef BILLBOARD_TRCHNIQUE_H
#define	BILLBOARD_TRCHNIQUE_H

#pragma once

#include "technique.h"
#include "math3d.h"

class BillboardTechnique : public Technique
{
public:

  BillboardTechnique ( );

  virtual bool Init ( );

  void SetVP ( const Matrix4f& VP );
  void SetCameraPosition ( const Vector3f& pos );
  void SetColorTextureUnit ( unsigned int textureUnit );

private:

  GLuint m_VPLocation;
  GLuint m_cameraPosLocation;
  GLuint m_colorMapLocation;
};

#endif /* BILLBOARD_TRCHNIQUE_H */

