#pragma once

#include <stdlib.h>


#include <GL/glew.h>
#include <GL/glut.h>


#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "glut_backend.h"
#include "lighting_technique.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

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
    m_pGameCamera ( nullptr ),
    m_pTexture    ( nullptr ),
    m_pEffect     ( nullptr ),
    m_Scale       ( 0.0f )
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
    Vector3f pos    ( 0.0f, 0.0f, -3.0f );
    Vector3f target ( 0.0f, 0.0f, 1.0f );
    Vector3f up     ( 0.0, 1.0f, 0.0f );

    m_pGameCamera = new Camera ( WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up );

    unsigned int indices[] = {
                                 0, 3, 1,
                                 1, 3, 2,
                                 2, 3, 0,
                                 1, 2, 0
                             };

    CreateIndexBuffer ( indices, sizeof ( indices ) );

    // Создаем буфер вершин
    CreateVertexBuffer ( indices, ARRAY_SIZE_IN_ELEMENTS ( indices )  );
    

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
    p.WorldPos ( 0.0f, 0.0f, 1.0f );
    p.SetCamera ( m_pGameCamera->GetPos ( ), m_pGameCamera->GetTarget ( ), m_pGameCamera->GetUp ( ) );
    p.SetPerspectiveProj ( 60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f );

    m_pEffect->SetWVP ( p.GetWVPTrans ( ) );
    const Matrix4f& WorldTransformation = p.GetWorldTrans ( );

    m_pEffect->SetWorldMatrix ( WorldTransformation );
    m_pEffect->SetDirectionalLight ( m_directionalLight );

    m_pEffect->SetEyeWorldPos ( m_pGameCamera->GetPos ( ) );
    m_pEffect->SetMatSpecularIntensity ( 1.0f );
    m_pEffect->SetMatSpecularPower ( 32.0f );

    // Подключаем атрибут (так же как и в вершинном шейдере)
    // 1: номер атрибута
    glEnableVertexAttribArray ( 0 );
    glEnableVertexAttribArray ( 1 );
    glEnableVertexAttribArray ( 2 );



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
    glVertexAttribPointer ( 2, 3, GL_FLOAT, GL_FALSE, sizeof ( Vertex ), ( const GLvoid* ) 20 );

    // Активный буфер
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_IBO );

    // Привязка текстуры к "модулю тектуры"
    m_pTexture->Bind ( GL_TEXTURE0 );

    // Рисуем что либо
    glDrawElements ( GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0 );

    // Отключаем атрибуты
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
    Vertex vertices[4] = {  Vertex ( Vector3f ( -1.0f, -1.0f, 0.5773f ),  Vector2f ( 0.0f, 0.0f ) ),
                            Vertex ( Vector3f ( 0.0f, -1.0f, -1.15475 ),  Vector2f ( 0.5f, 0.0f ) ),
                            Vertex ( Vector3f ( 1.0f, -1.0f, 0.5773f ),   Vector2f ( 1.0f, 0.0f ) ),
                            Vertex ( Vector3f ( 0.0f, 1.0f, 0.0f ),       Vector2f ( 0.5f, 1.0f ) ) };

    unsigned int vertexCount = ARRAY_SIZE_IN_ELEMENTS ( vertices );

    CalcNormals ( pIndices, indexCount, vertices, vertexCount );

    // Generate of buffer 
    //  | 1: кол VBO
    //  | 2: cсылка на масив для хранения ID буферов
    glGenBuffers ( 1, &m_VBO );

    // Привязка указателя к названию цели
    // Данный VBO становится активным
    glBindBuffer ( GL_ARRAY_BUFFER, m_VBO );

    // Выгружает данные в активный VBO
    // Выгрузка данных вершин в видеоустройство
    glBufferData ( GL_ARRAY_BUFFER, sizeof ( vertices ), vertices, GL_STATIC_DRAW );
  }

  void CreateIndexBuffer ( const unsigned int* pIndices, unsigned int sizeInBytes )
  {
    //// indeces
    //unsigned int indeces[] =
    //{ 
    //  0, 3, 1,
    //  1, 3, 2,
    //  2, 3, 0,
    //  1, 2, 0 // or 0, 1, 2 
    //};

    // Generate of buffer 
    //  | 1: кол VBO
    //  | 2: cсылка на масив для хранения ID буферов
    glGenBuffers ( 1, &m_IBO );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, m_IBO );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, pIndices, GL_STATIC_DRAW );
  }

  /*Эта функция принимает массив индексов, получает вершины треугольников, 
  полагаясь на них, и вычисляет нормали. В первом цикле мы только набираем 
  нормали для каждой тройки вершин. Для каждого треугольника она вычисляется 
  как векторное произведение двух сторон, которые получаются из вершин треугольника. 
  Перед добавлением нормаль в массив, ее нормируем. Причина в том, что результат 
  векторного произведения не обязательно единичный вектор. Во втором цикле мы 
  сканируем массив вершин напрямую (больше нам не нужны индексы) и нормируем 
  нормаль каждой вершины. Эта операция равносильна усреднению суммы нормалей и 
  выдает нам нормаль единичной длины. Функция вызывается до того, как создать буфер 
  вершин, что бы буфер нормалей был создан с другими атрибутами одновременно.*/
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


