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
    m_pGroundTex        ( nullptr ),
    m_pLightingEffect   ( nullptr ),
    m_pShadowMapEffect  ( nullptr ),
    m_scale             ( 0.0f ),
    m_specularPower     ( 0.5f ),
    m_specularIntensity ( 0.5f )
  {
    /*m_directionalLight.color            = Vector3f ( 1.0f );
    m_directionalLight.ambientIntensity = 1.0f;
    m_directionalLight.diffuseIntensity = 0.1f;
    m_directionalLight.direction        = Vector3f ( 1.0f, 0.0, 1.0 );*/
  
    m_spotLight.ambientIntensity = 0.3f;
    m_spotLight.diffuseIntensity = 1.3f;
    m_spotLight.color = Vector3f ( 1.0 );
    m_spotLight.attenuation.linear = 0.01f;
    m_spotLight.position = Vector3f ( -20.0f, 20.0f, 1.0f );
    m_spotLight.direction = Vector3f ( 1.0f, -1.0f, 0.0f );
    m_spotLight.cutOff = 20.0f;
  }

  ~Main ( )
  {
    SafeDelete ( m_pGameCamera );
    SafeDelete ( m_pLightingEffect );
    SafeDelete ( m_pMesh );
    SafeDelete ( m_pQuad );
    SafeDelete ( m_pGroundTex );
    SafeDelete ( m_pShadowMapEffect );
  }

  bool Init ( )
  {
    if ( !m_shadowMapFBO.Init ( WINDOW_WIDTH, WINDOW_HEIGHT ) ) 
    {
      return false;
    }

    Vector3f pos    ( 3.0f, 8.0f, -10.0 );
    Vector3f target ( 0.0f, -2.0f, 1.0f );
    Vector3f up     ( 0.0f, 1.0f, 0.0f );
    m_pGameCamera = new Camera ( WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up );

    
    #pragma region LIGHT EFFECT

    m_pLightingEffect = new LightingTechnique ( );

    if ( !m_pLightingEffect->Init ( ) )
    {
      printf ( "Error initializing the lighting technique\n" );
      return false;
    }

    m_pLightingEffect->Enable ( );

    m_pLightingEffect->SetSpotLights ( 1, &m_spotLight );
    m_pLightingEffect->SetTextureUnit ( 0 );
    m_pLightingEffect->SetShadowMapTextureUnit ( 1 );

    #pragma endregion
    
    #pragma region SHADOW MAP EFFECT

    m_pShadowMapEffect = new ShadowMapTechnique ( );

    if ( !m_pShadowMapEffect->Init ( ) )
    {
      printf ( "Error initializing the shadow map technique\n" );
      return false;
    }

    #pragma endregion

    #pragma region MESHES INIT


    // Model
    m_pMesh = new Mesh ( );

    if ( !m_pMesh->LoadMesh ( "Content/ship/phoenix_ugv.md2" ) ) //knight.x   ship/phoenix_ugv.md2   mon/Кровосос.obj 
    {
      return false;
    }

    #pragma endregion


    // Ground model
    m_pQuad = new Mesh ( );

    if ( !m_pQuad->LoadMesh ( "Content/quad.obj" ) )
    {
      return false;
    }

    m_pGroundTex = new Texture ( GL_TEXTURE_2D, "Content/metal.jpg" );

    if ( !m_pGroundTex->Load ( ) )
    {
      return false;
    }

    

    return true;

  }



  void Run ( )
  {
    GLUTBackendRun ( this );
  }


  virtual void RenderSceneCB ( )
  {
    m_pGameCamera->OnRender ( );
    m_scale += 0.05f;

    ShadowMapPass ( );
    RenderPass ( );

    glutSwapBuffers ( );
  }


  virtual void ShadowMapPass ( )
  {
    m_shadowMapFBO.BindForWriting ( );

    glClear ( GL_DEPTH_BUFFER_BIT );
    
    m_pShadowMapEffect->Enable ( );
    
    Pipeline p;
    p.Scale		  ( 0.1f, 0.1f, 0.1f );
    p.Rotate	  ( 0.0f, m_scale, 0.0f );
    p.WorldPos	( 0.0f, 0.0f, 5.0f );
    p.SetCamera ( m_spotLight.position, m_spotLight.direction, Vector3f ( 0.0f, 1.0f, 0.0f ) );
    p.SetPerspectiveProj ( 60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f );

    m_pShadowMapEffect->SetWVP ( p.GetWVPTrans ( ) );
    m_pMesh->Render ( );

    glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
  }



  virtual void RenderPass ( )
  {
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    m_pLightingEffect->Enable ( );

    m_shadowMapFBO.BindForReading ( GL_TEXTURE1 );

    Pipeline p;
    
    p.Scale     ( 10.0f, 10.0f, 10.0f );
    p.WorldPos  ( 0.0f, 0.0f, 1.0f );
    p.Rotate    ( 90.0f, 0.0f, 0.0f );

    p.SetPerspectiveProj ( 60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f );

    // Матрица трансформации с позиции камеры
    p.SetCamera ( m_pGameCamera->GetPos ( ), m_pGameCamera->GetTarget ( ), m_pGameCamera->GetUp ( ) );
    m_pLightingEffect->SetWVP ( p.GetWVPTrans ( ) );
    m_pLightingEffect->SetWorldMatrix ( p.GetWorldTrans ( ) );

    // Матрица трансформации с позиции точечного света 
    p.SetCamera ( m_spotLight.position, m_spotLight.direction, Vector3f ( 0.0f, 1.0f, 0.0f ) );
    m_pLightingEffect->SetLightWVP ( p.GetWVPTrans ( ) );
    m_pLightingEffect->SetEyeWorldPos ( m_pGameCamera->GetPos ( ) );

    // Земля
    m_pGroundTex->Bind ( GL_TEXTURE0 );
    m_pQuad->Render ( );

    p.Scale ( 0.1f, 0.1f, 0.1f );
    p.Rotate ( 0.0f, m_scale, 0.0f );
    p.WorldPos ( 0.0f, 0.0f, 3.0f );

    p.SetCamera ( m_pGameCamera->GetPos ( ), m_pGameCamera->GetTarget ( ), m_pGameCamera->GetUp ( ) );
    m_pLightingEffect->SetWVP ( p.GetWVPTrans ( ) );
    m_pLightingEffect->SetWorldMatrix ( p.GetWorldTrans ( ) );

    p.SetCamera ( m_spotLight.position, m_spotLight.direction, Vector3f ( 0.0f, 1.0f, 0.0f ) );
    m_pLightingEffect->SetLightWVP ( p.GetWVPTrans ( ) );

    // Model
    m_pMesh->Render ( );

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

    case 'й':
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
  Texture*  m_pGroundTex;

  LightingTechnique*  m_pLightingEffect;
  DirectionLight      m_directionalLight;

  ShadowMapFBO        m_shadowMapFBO;
  SpotLight           m_spotLight;
  ShadowMapTechnique* m_pShadowMapEffect;

  float m_specularPower;
  float m_specularIntensity;

  float m_scale;
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


