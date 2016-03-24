#ifndef BILLBOARD_LIST_H
#define	BILLBOARD_LIST_H

#pragma once

#include <string>
#include "billboard_technique.h"
#include "texture.h"


class BillboardList
{
public:

  BillboardList ( );
  ~BillboardList ( );

  bool Init ( const std::string& texFilename );

  void Render ( const Matrix4f& VP, const Vector3f& cameraPos );


private:

  void CreatePositionBuffer ( );

  GLuint m_VB;

  Texture* m_pTexture;

  BillboardTechnique m_technique;
};

#endif /* BILLBOARD_LIST_H */

