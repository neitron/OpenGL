#pragma once


#include "main.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


Main::Main ( ) :
  m_pGameCamera       ( nullptr ),
  m_pMesh             ( nullptr ),
  m_pGroundTex        ( nullptr ),
  m_pQuad             ( nullptr ),
  m_pModelTex         ( nullptr ),
  m_pNormalMap        ( nullptr ),
  m_pLightingEffect   ( nullptr ),
  m_pSkyBox           ( nullptr ),
  m_scale             ( 0.0f ),
  m_specularPower     ( 32.0f ),
  m_specularIntensity ( 1.0f )
{
  m_directionalLight.color            = Vector3f ( 1.0f );
  m_directionalLight.ambientIntensity = 0.7f;
  m_directionalLight.diffuseIntensity = 0.75f;
  m_directionalLight.direction        = Vector3f ( -1.0f, 0.0, -1.0 );
  
  m_persProjInfo.FOV    = 60.0f;
  m_persProjInfo.height = WINDOW_HEIGHT;
  m_persProjInfo.width  = WINDOW_WIDTH;
  m_persProjInfo.zNear  = 1.0f;
  m_persProjInfo.zFar   = 100.0f;
}

Main::~Main ( )
{
  SafeDelete ( m_pGameCamera );
  SafeDelete ( m_pLightingEffect );
  SafeDelete ( m_pMesh );
  SafeDelete ( m_pModelTex );
  SafeDelete ( m_pNormalMap );
  SafeDelete ( m_pQuad );
  SafeDelete ( m_pGroundTex );
  SafeDelete ( m_pSkyBox );
}

bool Main::Init ( )
{
  Vector3f pos    ( 28.0f, 26.0f, 27.0f );
  Vector3f target ( -0.6f, -0.6f, -0.6f );
  Vector3f up     ( 0.0f, 1.0f,  0.0f );
  m_pGameCamera = new Camera ( WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up );


#pragma region LIGHT EFFECT

  m_pLightingEffect = new LightingTechnique ( );

  if ( !m_pLightingEffect->Init ( ) )
  {
    printf ( "Error initializing the lighting technique\n" );
    return false;
  }

  m_pLightingEffect->Enable ( );
  m_pLightingEffect->SetColorTextureUnit ( 0 );
  m_pLightingEffect->SetNormalMapTextureUnit ( 2 );

  m_isUseNormalMap = true;
  m_pLightingEffect->SetUseNormalMap ( m_isUseNormalMap );

#pragma endregion

#pragma region MESHES INIT

  // Model
  m_pMesh = new Mesh ( );

  if ( !m_pMesh->LoadMesh ( "Content/mon/Кровосос.obj" ) ) // knight.x   ship/phoenix_ugv.md2   mon/Кровосос.obj  box.obj
  {
    return false;
  }

  m_pModelTex = new Texture ( GL_TEXTURE_2D, "Content/mon/act_krov.jpg" ); // bricks.jpg

  if ( !m_pModelTex->Load ( ) )
  {
    return false;
  }

  //m_pModelTex->Bind ( COLOR_TEXTURE_UNIT );

  m_pNormalMap = new Texture ( GL_TEXTURE_2D, "Content/mon/act_krov_normal.jpg" );

  if ( !m_pNormalMap->Load ( ) )
  {
    return false;
  }
  
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

  if ( !m_billboardList.Init ( "Content/target.png" ) ) // Content/monster_hellknight.png
  {
    return false;
  }

#pragma endregion

#pragma region SKYBOX

  m_pSkyBox = new SkyBox ( m_pGameCamera, m_persProjInfo );

  if ( !m_pSkyBox->Init ( ".",
    "Content/SkyBox/sky_right.jpg",
    "Content/SkyBox/sky_left.jpg",
    "Content/SkyBox/sky_top.jpg",
    "Content/SkyBox/sky_bot.jpg",
    "Content/SkyBox/sky_front.jpg",
    "Content/SkyBox/sky_back.jpg" ) ) 
  {
    return false;
  }

#pragma endregion

  return true;
}

void Main::Run ( )
{
  GLUTBackendRun ( this );
}

void Main::RenderSceneCB ( )
{
  m_pGameCamera->OnRender ( );
  m_scale += 0.01f;

  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  m_pLightingEffect->Enable ( );

  Pipeline p;
  p.Scale     ( 20.0f, 20.0f, 20.0f );
  p.WorldPos  ( 0.0f,  0.0f,  1.0f  );
  p.Rotate    ( 90.0f, 0.0f,  5.0f  );
  p.SetCamera ( m_pGameCamera->GetPos ( ), m_pGameCamera->GetTarget ( ), m_pGameCamera->GetUp ( ) );
  p.SetPerspectiveProj ( m_persProjInfo );

  m_pLightingEffect->SetUseNormalMap ( false );

  m_pLightingEffect->SetWVP ( p.GetWVPTrans ( ) );
  m_pLightingEffect->SetWorldMatrix ( p.GetWorldTrans ( ) );
  m_pLightingEffect->SetEyeWorldPos ( m_pGameCamera->GetPos ( ) );
  
  m_pLightingEffect->SetDirectionalLight ( m_directionalLight );

  m_pLightingEffect->SetMatSpecularIntensity ( m_specularIntensity );
  m_pLightingEffect->SetMatSpecularPower     ( m_specularPower );

  // Земля
  m_pGroundTex->Bind ( COLOR_TEXTURE_UNIT );
  m_pQuad->Render ( );

  m_pLightingEffect->SetUseNormalMap ( m_isUseNormalMap );

  p.Scale     ( 0.1f, 0.1f, 0.1f );
  p.Rotate    ( 0.0f, m_scale, 0.0f );
  p.WorldPos  ( 0.0f, 0.0f, 0.0f );
  m_pLightingEffect->SetWVP ( p.GetWVPTrans ( ) );
  m_pLightingEffect->SetWorldMatrix ( p.GetWorldTrans ( ) );

  // Model
  m_pModelTex->Bind ( COLOR_TEXTURE_UNIT );
  m_pNormalMap->Bind ( NORMAL_TEXTURE_UNIT );
  m_pMesh->Render ( );

  // billboards
  p.Scale ( 10.0f, 10.0f, 10.0f );
  p.WorldPos ( 0.0f, 0.0f, 0.0f );
  m_billboardList.Render ( p.GetVPTrans ( ), m_pGameCamera->GetPos ( ) );

  m_pSkyBox->Render ( );

  glutSwapBuffers ( );
}

void Main::IdleCB ( )
{
  RenderSceneCB ( );
}

void Main::SpecialKeyboardCB ( int key, int x, int y )
{
  m_pGameCamera->OnKeyboard ( key );
}

void Main::KeyboardCB ( unsigned char key, int x, int y )
{
  Vector3f camPos ( m_pGameCamera->GetPos ( ) );
  Vector3f camTar ( m_pGameCamera->GetTarget ( ) );
  switch ( key )
  {
  case 'q':
    std::exit ( EXIT_SUCCESS );
    break;

  case '+':
    //Camera::STEP_SIZE_MOUSE *= 10;
    //m_spotLight.position.x++;
    break;

  case '-':
    //Camera::STEP_SIZE_MOUSE /= 10;
    //m_spotLight.position.x--;
    break;

  case 'w':
    m_isUseNormalMap = !m_isUseNormalMap;
    break;

  case 'a':
  {
    m_directionalLight.ambientIntensity += 0.05f;
    printf ( "\nambientIntensity: %f", m_directionalLight.ambientIntensity );
    break;
  }
  case 's':
  {
    if ( m_directionalLight.ambientIntensity >= 0.05f )
      m_directionalLight.ambientIntensity -= 0.05f;
    else
      m_directionalLight.diffuseIntensity = 0;

    printf ( "\nambientIntensity: %f", m_directionalLight.ambientIntensity );

    break;
  }
  case 'd':
  {
    m_directionalLight.diffuseIntensity += 0.05f;
    printf ( "\ndiffuseIntensity: %f", m_directionalLight.diffuseIntensity );
    break;
  }
  case 'f':
  {
    if ( m_directionalLight.diffuseIntensity >= 0.05f )
      m_directionalLight.diffuseIntensity -= 0.05f;
    else
      m_directionalLight.diffuseIntensity = 0;

    printf ( "\ndiffuseIntensity: %f", m_directionalLight.diffuseIntensity );

    break;
  }
  case 'z':
  {
    m_specularPower *= 2.0f;
    printf ( "\nm_specularPower: %f", m_specularPower );
    break;
  }
  case 'x':
  {
    if ( m_specularPower > 1.0f )
      m_specularPower /= 2.0f;
    else
      m_specularPower = 1.0f;

    printf ( "\nm_specularPower: %f", m_specularPower );

    break;
  }
  case 'c':
  {
    m_specularIntensity += 0.05f;
    printf ( "\nm_specularIntensity: %f", m_specularIntensity );
    break;
  }
  case 'v':
  {
    if ( m_specularIntensity >= 0.05f )
      m_specularIntensity -= 0.05f;
    else
      m_specularIntensity = 0;

    printf ( "\nm_specularIntensity: %f", m_specularIntensity );

    break;
  }
  }
}

void Main::PassiveMouseCB ( int x, int y )
{
  m_pGameCamera->OnMouse ( x, y );
}
