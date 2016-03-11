#pragma once

#include <stdlib.h>


#include <GL/glew.h>
#include <GL/glut.h>


#include "pipeline.h"
#include "camera.h"
#include "glut_backend.h"
#include "lighting_technique.h"
#include "utils.h"
#include "mesh.h"
#include "shadow_map_fbo.h"
#include "shadow_map_technique.h"



const int WINDOW_WIDTH    = 1300;
const int WINDOW_HEIGHT   = 700;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Main : public ICallbacks
{

public:

  Main ( ) :
    m_pGameCamera       ( nullptr ),
    m_pMesh             ( nullptr ),
    m_pQuad             ( nullptr ),
    m_pEffect           ( nullptr ),
    m_pShadowMapTech    ( nullptr ),
    m_Scale             ( 0.0f ),
    m_specularPower     ( 0.0f ),
    m_specularIntensity ( 0.0f )
  {
    m_directionalLight.color            = Vector3f ( 1.0f );
    m_directionalLight.ambientIntensity = 1.0f;
    m_directionalLight.diffuseIntensity = 0.1f;
    m_directionalLight.direction        = Vector3f ( 1.0f, 0.0, 1.0 );
  
    m_spotLight.ambientIntensity = 0.0f;
    m_spotLight.diffuseIntensity = 0.9f;
    m_spotLight.color = Vector3f ( 1.0 );
    m_spotLight.attenuation.linear = 0.01f;
    m_spotLight.position = Vector3f ( -20.0f, 20.0f, 5.0f );
    m_spotLight.direction = Vector3f ( 1.0f, -1.0f, 0.0f );
    m_spotLight.cutOff = 20.0f;
  }

  ~Main ( )
  {
    SafeDelete ( m_pGameCamera );
    SafeDelete ( m_pEffect );
    SafeDelete ( m_pMesh );
    SafeDelete ( m_pQuad );
    SafeDelete ( m_pShadowMapTech );
  }

  bool Init ( )
  {
    if ( !m_shadowMapFBO.Init ( WINDOW_WIDTH, WINDOW_HEIGHT ) ) 
    {
      return false;
    }

    m_pGameCamera = new Camera ( WINDOW_WIDTH, WINDOW_HEIGHT );

    m_pEffect = new LightingTechnique ( );

    if ( !m_pEffect->Init ( ) )
    {
      printf ( "Error initializing the lighting technique\n" );
      return false;
    }

    m_pShadowMapTech = new ShadowMapTechnique ( );

    if ( !m_pShadowMapTech->Init ( ) ) 
    {
      printf ( "Error initializing the shadow map technique\n" );
      return false;
    }

    //m_pEffect->SetTextureUnit ( 0 );

    m_pQuad = new Mesh ( );

    if ( !m_pQuad->LoadMesh ( "Content/quad.obj" ) ) 
    {
      return false;
    }

    m_pMesh = new Mesh ( );

    return m_pMesh->LoadMesh ( "Content/ship/phoenix_ugv.md2" ); //knight.x   phoenix_ugv.md2
  }

  void Run ( )
  {
    GLUTBackendRun ( this );
  }

  virtual void RenderSceneCB ( )
  {
    m_pGameCamera->OnRender ( );
    m_Scale += 0.05f;

    ShadowMapPass ( );
    RenderPass ( );

    glutSwapBuffers ( );
  }

  virtual void ShadowMapPass ( )
  {
    m_shadowMapFBO.BindForWriting ( );

    glClear ( GL_DEPTH_BUFFER_BIT );
    
    m_pShadowMapTech->Enable ( );
    
    Pipeline p;
    p.Scale ( 0.2f, 0.2f, 0.2f );
    p.Rotate ( 0.0f, m_Scale, 0.0f );
    p.WorldPos ( 0.0f, 0.0f, 5.0f );
    p.SetCamera ( m_spotLight.position, m_spotLight.direction, Vector3f ( 0.0f, 1.0f, 0.0f ) );
    p.SetPerspectiveProj ( 60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f );

    m_pShadowMapTech->SetWVP ( p.GetWVPTrans ( ) );
    m_pMesh->Render ( );

    glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
  }

  virtual void RenderPass ( )
  {
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //m_pEffect->Enable ( );

    m_pShadowMapTech->SetTextureUnit ( 0 );
    m_shadowMapFBO.BindForReading ( GL_TEXTURE0 );

    Pipeline p;
    p.Scale ( 5.0f, 5.0f, 5.0f );
    p.WorldPos ( 0.0f, 0.0f, 10.0f );
    p.SetCamera ( m_pGameCamera->GetPos ( ), m_pGameCamera->GetTarget ( ), m_pGameCamera->GetUp ( ) );
    p.SetPerspectiveProj ( 60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f );
    
    m_pShadowMapTech->SetWVP ( p.GetWVPTrans ( ) );
    
    m_pQuad->Render ( );
  }
  
  virtual void IdleCB ( )
  {
    RenderSceneCB ( );
  }

  virtual void SpecialKeyboardCB ( int key, int x, int y )
  {
    m_pGameCamera->OnKeyboard ( key );
  }

  virtual void KeyboardCB ( unsigned char key, int x, int y )
  {
    switch ( key )
    {
    case 'q':
      std::exit ( EXIT_SUCCESS );
      break;

    case '+':
      //Camera::STEP_SIZE_MOUSE *= 10;
      break;

    case '-':
      //Camera::STEP_SIZE_MOUSE /= 10;
      break;

    case 'a':
      m_directionalLight.ambientIntensity += 0.05f;
      break;

    case 's':
      m_directionalLight.ambientIntensity -= 0.05f;
      break;

    case 'z':
      m_directionalLight.diffuseIntensity += 0.05f;
      break;

    case 'x':
      m_directionalLight.diffuseIntensity -= 0.05f;
      break;

    case 'd':
      m_specularPower *= 2.0f;
      break;

    case 'f':
      m_specularPower /= 2.0f;
      break;

    case 'c':
      m_specularIntensity += 0.05f;
      break;

    case 'v':
      m_specularIntensity -= 0.05f;
      break;
    }
  }

  virtual void PassiveMouseCB ( int x, int y )
  {
    m_pGameCamera->OnMouse ( x, y );
  }

private:

  Camera*   m_pGameCamera;

  Mesh*     m_pMesh;
  Mesh*     m_pQuad;

  LightingTechnique* m_pEffect;
  DirectionLight m_directionalLight;

  ShadowMapFBO m_shadowMapFBO;
  SpotLight m_spotLight;
  ShadowMapTechnique* m_pShadowMapTech;

  float m_specularPower;
  float m_specularIntensity;

  float m_Scale;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  GLUTBackendInit ( argc, argv );

  if ( !GLUTBackendCreateWindow ( WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "OpenGL 3.X Neitron" ) )
  {
    return 1;
  }

  Main* pApp = new Main ( );

  if ( !pApp->Init ( ) )
  {
    return 1;
  }

  pApp->Run ( );

  delete pApp;

  return 0;
}


