#ifndef SKYBOX_H
#define SKYBOX_H

#pragma once

#include <string>

#include "camera.h"
#include "skybox_technique.h"
#include "cubemap_texture.h"
#include "mesh.h"

using std::string;

class SkyBox
{
public:
  
  SkyBox ( const Camera* pCamera, const PersProjInfo& p );

  ~SkyBox ( );

  bool Init ( const string& Directory,
    const string& PosXFilename,
    const string& NegXFilename,
    const string& PosYFilename,
    const string& NegYFilename,
    const string& PosZFilename,
    const string& NegZFilename );

  void Render ( );

private:
  
  const Camera*   m_pCamera;
  PersProjInfo    m_persProjInfo;

  SkyboxTechnique* m_pSkyboxTechnique;
  
  CubemapTexture* m_pCubemapTex;
  Mesh*           m_pMesh;
};

#endif /* SKYBOX_H */

