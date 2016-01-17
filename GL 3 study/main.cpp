#pragma once

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <assert.h>

#include <GL/glew.h>
#include <GL/glut.h>


#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "glut_backend.h"
#include "lighting_technique.h"

const int WINDOW_WIDTH    = 1366;
const int WINDOW_HEIGHT   = 768;

struct Vertex
{
  Vector3f m_pos;
  Vector2f m_tex;

  Vertex ( ) { }
  Vertex ( Vector3f pos, Vector2f tex ) :
    m_pos ( pos ),
    m_tex ( tex ) { }
};

GLuint gWVPLocation;
GLuint gSampler;
GLuint gWave;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Main : public ICallbacks
{

public:

  Main ( ) :
    m_pGameCamera ( nullptr ),
    m_pTexture    ( nullptr ),
    m_pEffect     ( nullptr ),
    m_Scale       ( 0.0f )
  {
    m_directionalLight.color = Vector3f ( 1.0f, 1.0f, 1.0f );
    m_directionalLight.ambientIntensity = 0.5;
  }

  ~Main ( )
  {
    delete m_pGameCamera;
    delete m_pTexture;
    delete m_pEffect;
  }

  bool Init ( )
  {
    m_pGameCamera = new Camera ( WINDOW_WIDTH, WINDOW_HEIGHT );

    // Создаем буфер вершин
    CreateVertexBuffer ( );
    CreateIndexBuffer ( );

    m_pEffect = new LightingTechnique ( );

    if ( !m_pEffect->Init ( ) )
    {
      return false;
    }

    m_pEffect->Eneble ( );

    m_pEffect->SetTextureUnit ( 0 );

    m_pTexture = new Texture ( GL_TEXTURE_2D, "Content/test_01.png" );

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

    glClear ( GL_COLOR_BUFFER_BIT );

    m_Scale += 0.01;

    Pipeline p;
    p.Rotate ( 0.0f, m_Scale, 0.0f );
    p.WorldPos ( 0.0f, 0.0f, 3.0f );
    p.SetCamera ( m_pGameCamera->GetPos ( ), m_pGameCamera->GetTarget ( ), m_pGameCamera->GetUp ( ) );
    p.SetPerspectiveProj ( 60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f );

    m_pEffect->SetWVP ( p.GetTrans ( ) );
    m_pEffect->SetDirectionalLight ( m_directionalLight );


    // Подключаем атрибут (так же как и в вершинном шейдере)
    // 1: номер атрибута
    glEnableVertexAttribArray ( 0 );
    glEnableVertexAttribArray ( 1 );

    // Активный буфер
    glBindBuffer ( GL_ARRAY_BUFFER, m_VBO );

    // Как конвейеру воспринимать данные буфера (каждый атрибут в отдельности)
    // 1: номер атрибута (атрибут позиции, цвета или текстуры)
    // 2: количество компонент в атрибуте (x y z) = 3
    // 3: тип каждого компонента (x: float)
    // 4: нормализировать атрибуты или нет
    // 5: число байтов между 2 атрибутами (одного типа)
    // 6: смещение в структуре (с какой позиции начинаются данные даного атрибута)
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), nullptr );
    glVertexAttribPointer ( 1, 2, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), reinterpret_cast<const GLvoid*>( sizeof ( GLfloat ) * 3 ) );

    // Активный буфер
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_IBO );

    // Привязка текстуры к "модулю тектуры"
    m_pTexture->Bind ( GL_TEXTURE0 );

    // Рисуем что либо
    glDrawElements ( GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0 );

    // Отключаем атрибуты
    glDisableVertexAttribArray ( 0 );
    glDisableVertexAttribArray ( 1 );

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
      exit ( 1 );
      break;

    case '+':
      //Camera::STEP_SIZE_MOUSE *= 10;
      break;

    case '-':
      //Camera::STEP_SIZE_MOUSE /= 10;
      break;
    }
  }

  virtual void PassiveMouseCB ( int x, int y )
  {
    m_pGameCamera->OnMouse ( x, y );
  }

private:

  void CreateVertexBuffer ( )
  {
    // 3 vertices
    Vertex Vertices[4] = 
    { 
      Vertex ( Vector3f ( -1.0f, -1.0f, 0.5773f ),  Vector2f ( 0.0f, 1.0f ) ),
      Vertex ( Vector3f ( 0.0f, -1.0f, -1.15475 ),  Vector2f ( 0.5f, 1.0f ) ),
      Vertex ( Vector3f ( 1.0f, -1.0f, 0.5773f ),   Vector2f ( 1.0f, 1.0f ) ),
      Vertex ( Vector3f ( 0.0f, 1.0f, 0.0f ),       Vector2f ( 0.5f, 0.0f ) ) 
    };

    // Generate of buffer 
    //  | 1: кол VBO
    //  | 2: cсылка на масив для хранения ID буферов
    glGenBuffers ( 1, &m_VBO );

    // Привязка указателя к названию цели
    // Данный VBO становится активным
    glBindBuffer ( GL_ARRAY_BUFFER, m_VBO );

    // Выгружает данные в активный VBO
    // Выгрузка данных вершин в видеоустройство
    glBufferData ( GL_ARRAY_BUFFER, sizeof ( Vertices ), Vertices, GL_STATIC_DRAW );
  }

  void CreateIndexBuffer ( )
  {
    // indeces
    unsigned int indeces[] =
    { 
      0, 3, 1,
      1, 3, 2,
      2, 3, 0,
      1, 2, 0 // or 0, 1, 2 
    };

    // Generate of buffer 
    //  | 1: кол VBO
    //  | 2: cсылка на масив для хранения ID буферов
    glGenBuffers ( 1, &m_IBO );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_IBO );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( indeces ), indeces, GL_STATIC_DRAW );
  }


  GLuint    m_VBO; // переменная для хранения указателя на буфер вершин
  GLuint    m_IBO;

  Texture*  m_pTexture;
  Camera*   m_pGameCamera;

  LightingTechnique* m_pEffect;
  DirectionLight m_directionalLight;

  float m_Scale;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  GLUTBackendInit ( argc, argv );

  if ( !GLUTBackendCreateWindow ( WINDOW_WIDTH, WINDOW_HEIGHT, 32, true, "OpenGL 3.X Neitron" ) )
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


