#pragma once

#include <stdlib.h>


#include <GL/glew.h>
#include <GL/glut.h>


#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "glut_backend.h"
#include "lighting_technique.h"
#include "utils.h"


const int WINDOW_WIDTH    = 1300;
const int WINDOW_HEIGHT   = 700;

struct Vertex
{
  Vector3f m_pos;
  Vector2f m_tex;
  Vector3f m_normal;

  Vertex ( ) { }
  Vertex ( Vector3f pos, Vector2f tex ) :
    m_pos     ( pos ),
    m_tex     ( tex ),
    m_normal  ( Vector3f ( 0.0f, 0.0f, 0.0f ) ) { }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Main : public ICallbacks
{

public:

  Main ( ) :
    m_pGameCamera       ( nullptr ),
    m_pTexture          ( nullptr ),
    m_pEffect           ( nullptr ),
    m_Scale             ( 0.0f ),
    m_specularPower     ( 32.0f ),
    m_specularIntensity ( 1.0f )
  {
    m_directionalLight.color            = Vector3f ( 1.0f, 1.0f, 1.0f );
    m_directionalLight.ambientIntensity = 0.0f;
    m_directionalLight.diffuseIntensity = 0.75f;
    m_directionalLight.direction        = Vector3f ( 1.0f, 0.0, 0.0 );
  }

  ~Main ( )
  {
    delete m_pGameCamera;
    delete m_pTexture;
    delete m_pEffect;
  }

  bool Init ( )
  {
    Vector3f pos    ( -10.0f, 0.0f, -10.0f );
    Vector3f target ( 1.0f, 0.0f, 1.0f );
    Vector3f up     ( 0.0, 1.0f, 0.0f );

    m_pGameCamera = new Camera ( WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up );

    unsigned int indices[] = {
                                 0, 2, 1,
                                 0, 3, 2
                             };

    CreateIndexBuffer ( indices, sizeof ( indices ) );

    // ������� ����� ������
    CreateVertexBuffer ( indices, ARRAY_SIZE_IN_ELEMENTS ( indices )  );
    

    m_pEffect = new LightingTechnique ( );

    if ( !m_pEffect->Init ( ) )
    {
      return false;
    }

    m_pEffect->Eneble ( );

    m_pEffect->SetTextureUnit ( 0 );

    m_pTexture = new Texture ( GL_TEXTURE_2D, "Content/metal.jpg" );

    if ( !m_pTexture->Load ( ) )
      return false;

    return true;
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

    
    // ������� ��� �������� ������� ������������
    Pipeline p;
    p.Rotate ( 0.0f, m_Scale, 0.0f );
    p.WorldPos ( 0.0f, 0.0f, 1.0f );
    p.SetCamera ( m_pGameCamera->GetPos ( ), m_pGameCamera->GetTarget ( ), m_pGameCamera->GetUp ( ) );
    p.SetPerspectiveProj ( 60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f );

    // ������� �������������
    m_pEffect->SetWVP ( p.GetWVPTrans ( ) );
    const Matrix4f& WorldTransformation = p.GetWorldTrans ( );
    m_pEffect->SetWorldMatrix ( WorldTransformation );
    
    // ������������ ����
    m_pEffect->SetDirectionalLight ( m_directionalLight );

    // ���������� ����
    m_pEffect->SetEyeWorldPos ( m_pGameCamera->GetPos ( ) );
    m_pEffect->SetMatSpecularIntensity ( m_specularIntensity );
    m_pEffect->SetMatSpecularPower ( m_specularPower );


    #pragma region �������� ����

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
    
    #pragma region ������������ ����

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

    m_pEffect->SetSpotLights ( COUNT_OF_SPOT_LIGHTS, sl );

    #pragma endregion


    // ���������� ������� (��� �� ��� � � ��������� �������)
    // 1: ����� ��������
    glEnableVertexAttribArray ( 0 );
    glEnableVertexAttribArray ( 1 );
    glEnableVertexAttribArray ( 2 );

    // �������� �����
    glBindBuffer ( GL_ARRAY_BUFFER, m_VBO );

    // ��� ��������� ������������ ������ ������ (������ ������� � �����������)
    // 1: ����� �������� (������� �������, ����� ��� ��������)
    // 2: ���������� ��������� � �������� (x y z) = 3
    // 3: ��� ������� ���������� (x: float)
    // 4: ��������������� �������� ��� ���
    // 5: ����� ������ ����� 2 ���������� (������ ����)
    // 6: �������� � ��������� (� ����� ������� ���������� ������ ������ ��������)
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), nullptr );   // ������� ������ (���������)
    glVertexAttribPointer ( 1, 2, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), reinterpret_cast<const GLvoid*>( sizeof ( GLfloat ) * 3 ) ); // ��������������� ���������� �������� 
    glVertexAttribPointer ( 2, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( const GLvoid* ) 20 );    // ������� �������� ������

    // �������� �����
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_IBO );

    // �������� �������� � "������ �������"
    m_pTexture->Bind ( GL_TEXTURE0 );

    // ������ ��� ����
    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

    // ��������� ��������
    glDisableVertexAttribArray ( 0 );
    glDisableVertexAttribArray ( 1 );
    glDisableVertexAttribArray ( 2 );

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

  void CreateVertexBuffer ( const unsigned int* pIndices, unsigned int indexCount )
  {
    // 3 vertices
    Vertex vertices[4] = {  Vertex ( Vector3f ( -10.0f, -2.0f, -10.0f ),    Vector2f ( 0.0f, 0.0f ) ),    //Vector2f ( 0.0f, 0.0f )
                            Vertex ( Vector3f (  10.0f, -2.0f, -10.0f ),    Vector2f ( 2.0f, 0.0f ) ),    //Vector2f ( 0.5f, 0.0f )
                            Vertex ( Vector3f (  10.0f, -2.0f,  10.0f ),    Vector2f ( 2.0f, 2.0f ) ),    //Vector2f ( 1.0f, 0.0f )
                            Vertex ( Vector3f ( -10.0f, -2.0f,  10.0f ),    Vector2f ( 0.0f, 2.0f ) ) };  //Vector2f ( 0.5f, 1.0f )

    unsigned int vertexCount = ARRAY_SIZE_IN_ELEMENTS ( vertices );

    CalcNormals ( pIndices, indexCount, vertices, vertexCount );

    // Generate of buffer 
    //  | 1: ��� VBO
    //  | 2: c����� �� ����� ��� �������� ID �������
    glGenBuffers ( 1, &m_VBO );

    // �������� ��������� � �������� ����
    // ������ VBO ���������� ��������
    glBindBuffer ( GL_ARRAY_BUFFER, m_VBO );

    // ��������� ������ � �������� VBO
    // �������� ������ ������ � ���������������
    glBufferData ( GL_ARRAY_BUFFER, sizeof ( vertices ), vertices, GL_STATIC_DRAW );
  }

  void CreateIndexBuffer ( const unsigned int* pIndices, unsigned int sizeInBytes )
  {
    // Generate of buffer 
    //  | 1: ��� VBO
    //  | 2: c����� �� ����� ��� �������� ID �������
    glGenBuffers ( 1, &m_IBO );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_IBO );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, pIndices, GL_STATIC_DRAW );
  }

  /*��� ������� ��������� ������ ��������, �������� ������� �������������, 
  ��������� �� ���, � ��������� �������. � ������ ����� �� ������ �������� 
  ������� ��� ������ ������ ������. ��� ������� ������������ ��� ����������� 
  ��� ��������� ������������ ���� ������, ������� ���������� �� ������ ������������. 
  ����� ����������� ������� � ������, �� ���������. ������� � ���, ��� ��������� 
  ���������� ������������ �� ����������� ��������� ������. �� ������ ����� �� 
  ��������� ������ ������ �������� (������ ��� �� ����� �������) � ��������� 
  ������� ������ �������. ��� �������� ����������� ���������� ����� �������� � 
  ������ ��� ������� ��������� �����. ������� ���������� �� ����, ��� ������� ����� 
  ������, ��� �� ����� �������� ��� ������ � ������� ���������� ������������.*/
  void CalcNormals ( const unsigned int* pIndices, unsigned int indexCount, Vertex* pVertices, unsigned int vertexCount )
  {
    for ( unsigned int i = 0; i < indexCount; i += 3 )
    {
      unsigned int Index0 = pIndices[i];
      unsigned int Index1 = pIndices[i + 1];
      unsigned int Index2 = pIndices[i + 2];

      Vector3f v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
      Vector3f v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
      Vector3f Normal = v1.Cross ( v2 );

      Normal.Normalize ( );

      pVertices[Index0].m_normal += Normal;
      pVertices[Index1].m_normal += Normal;
      pVertices[Index2].m_normal += Normal;
    }

    for ( unsigned int i = 0; i < vertexCount; i++ )
    {
      pVertices[i].m_normal.Normalize ( );
    }
  }

  GLuint    m_VBO; // ���������� ��� �������� ��������� �� ����� ������
  GLuint    m_IBO;

  Texture*  m_pTexture;
  Camera*   m_pGameCamera;

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

  system ( "pause" );
  return 0;
}


