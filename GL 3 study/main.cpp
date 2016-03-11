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


const int WINDOW_WIDTH    = 1300;
const int WINDOW_HEIGHT   = 700;
const int TEST = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Main : public ICallbacks
{

public:

  Main ( ) :
    m_pGameCamera       ( nullptr ),
    m_pMesh             ( nullptr ),
    m_pEffect           ( nullptr ),
    m_Scale             ( 0.0f ),
    m_specularPower     ( 0.0f ),
    m_specularIntensity ( 0.0f )
  {
    m_directionalLight.color            = Vector3f ( 1.0f, 1.0f, 1.0f );
    m_directionalLight.ambientIntensity = 1.0f;
    m_directionalLight.diffuseIntensity = 0.1f;
    m_directionalLight.direction        = Vector3f ( 1.0f, 0.0, 1.0 );
  }

  ~Main ( )
  {
    delete m_pGameCamera;
    delete m_pEffect;
    delete m_pMesh;
  }

  bool Init ( )
  {
    Vector3f pos    ( -10.0f, 4.0f, 0.0f );
    Vector3f target ( 1.0f, 0.0f, 1.0f );
    Vector3f up     ( 0.0, 1.0f, 0.0f );

    m_pGameCamera = new Camera ( WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up );

    m_pEffect = new LightingTechnique ( );

    if ( !m_pEffect->Init ( ) )
    {
      printf ( "Error initializing the lighting technique\n" );
      return false;
    }

    m_pEffect->Eneble ( );

    m_pEffect->SetTextureUnit ( 0 );

    m_pMesh = new Mesh ( );

    return m_pMesh->LoadMesh ( "Content/phoenix_ugv.md2" ); //knight.x   phoenix_ugv.md2
  }

  void Run ( )
  {
    GLUTBackendRun ( this );
  }

  virtual void RenderSceneCB ( )
  {
    m_pGameCamera->OnRender ( );

    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_Scale += 0.01;

    float scale = 0.1f;
    // Конвеер для создания матрици трасформаций
    Pipeline p;
    p.Scale ( scale, scale, scale );
    p.Rotate ( 0.0f, m_Scale, 0.0f );
    p.WorldPos ( 0.0f, 0.0f, 10.0f );
    p.SetCamera ( m_pGameCamera->GetPos ( ), m_pGameCamera->GetTarget ( ), m_pGameCamera->GetUp ( ) );
    p.SetPerspectiveProj ( 60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f );

    // Матрици трансформаций
    m_pEffect->SetWVP ( p.GetWVPTrans ( ) );
    m_pEffect->SetWorldMatrix ( p.GetWorldTrans ( ) );
    
    // Направленный свет
    m_pEffect->SetDirectionalLight ( m_directionalLight );

    // Отраженный свет
    m_pEffect->SetEyeWorldPos ( m_pGameCamera->GetPos ( ) );
    m_pEffect->SetMatSpecularIntensity ( m_specularIntensity );
    m_pEffect->SetMatSpecularPower ( m_specularPower );


    #pragma region Точечный свет

    const int COUNT_OF_POINT_LIGHTS = 3;
    PointLight pl[COUNT_OF_POINT_LIGHTS];

    pl[0].diffuseIntensity = 0.5f;
    pl[0].color = Vector3f ( 1.0f, 0.0f, 0.0f );
    pl[0].position = Vector3f ( sinf ( m_Scale ) * 10, 1.0f, cosf ( m_Scale ) * 10 );
    pl[0].attenuation.linear = 0.1f;

    pl[1].diffuseIntensity = 0.5f;
    pl[1].color = Vector3f ( 0.0f, 1.0f, 0.0f );
    pl[1].position = Vector3f ( sinf ( m_Scale + 2.1f ) * 10, 1.0f, cosf ( m_Scale + 2.1f ) * 10 );
    pl[1].attenuation.linear = 0.1f;

    pl[2].diffuseIntensity = 0.5f;
    pl[2].color = Vector3f ( 0.0f, 0.0f, 1.0f );
    pl[2].position = Vector3f ( sinf ( m_Scale + 4.2f ) * 10, 1.0f, cosf ( m_Scale + 4.2f ) * 10 );
    pl[2].attenuation.linear = 0.1f;

    //m_pEffect->SetPointLights ( COUNT_OF_POINT_LIGHTS, pl );

    #pragma endregion
    
    #pragma region Прожекторный свет

    const int COUNT_OF_SPOT_LIGHTS = 2;
    SpotLight sl[COUNT_OF_SPOT_LIGHTS];

    sl[0].diffuseIntensity = 15.0f;
    sl[0].color = Vector3f ( 1.0f, 1.0f, 0.7f );
    sl[0].position = Vector3f ( -0.0f, -1.9f, -0.0f );
    sl[0].direction = Vector3f ( sinf ( m_Scale ), 0.0f, cosf ( m_Scale ) );
    sl[0].attenuation.linear = 0.1f;
    sl[0].cutOff = 20.0f;

    sl[1].diffuseIntensity = 5.0f;
    sl[1].color = Vector3f ( 0.0f, 1.0f, 1.0f );
    sl[1].position = m_pGameCamera->GetPos ( );
    sl[1].direction = m_pGameCamera->GetTarget ( );
    sl[1].attenuation.linear = 0.1f;
    sl[1].cutOff = 10.0f;

    //m_pEffect->SetSpotLights ( COUNT_OF_SPOT_LIGHTS, sl );

    #pragma endregion
    
    m_pMesh->Render ( );

    glutSwapBuffers ( );
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

  GLuint    m_VBO; // переменная для хранения указателя на буфер вершин
  GLuint    m_IBO;

  Camera*   m_pGameCamera;

  Mesh*     m_pMesh;

  LightingTechnique* m_pEffect;
  DirectionLight m_directionalLight;

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


