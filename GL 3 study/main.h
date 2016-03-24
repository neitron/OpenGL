#ifndef MAIN_H
#define MAIN_H

#pragma once

#include <stdlib.h>


#include <GL/glew.h>
#include <GL/glut.h>

#include "engine_common.h"
#include "utils.h"
#include "glut_backend.h"
#include "pipeline.h"
#include "camera.h"
#include "lighting_technique.h"
#include "mesh.h"
#include "shadow_map_fbo.h"
#include "shadow_map_technique.h"
#include "skybox.h"
#include "billboard_list.h"

const int WINDOW_WIDTH = 1300;
const int WINDOW_HEIGHT = 700;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Main : public ICallbacks
{

public:

  Main ( );

  ~Main ( );

  bool Init ( );

  void Run ( );

  virtual void RenderSceneCB ( );

  // Рендер данных глубины в карту теней
  //virtual void ShadowMapPass ( );

  //virtual void RenderPass ( );

  virtual void IdleCB ( );

  virtual void SpecialKeyboardCB ( int key, int x, int y );

  virtual void KeyboardCB ( unsigned char key, int x, int y );

  virtual void PassiveMouseCB ( int x, int y );

private:

  Camera*   m_pGameCamera;

  // main temp model
  Mesh*     m_pMesh;
  Texture*  m_pModelTex;
  Texture*  m_pNormalMap;

  // ground model
  Mesh*     m_pQuad;
  Texture*  m_pGroundTex;

  LightingTechnique*  m_pLightingEffect;
  DirectionLight      m_directionalLight;

  ShadowMapFBO        m_shadowMapFBO;

  //SpotLight           m_spotLight;
  ShadowMapTechnique* m_pShadowMapEffect;

  SkyBox* m_pSkyBox;

  PersProjInfo m_persProjInfo;

  BillboardList m_billboardList;

  float m_specularPower;
  float m_specularIntensity;

  float m_scale;

  bool m_isUseNormalMap;
};


#endif /* MAIN_H */