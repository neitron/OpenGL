#pragma once

#include "billboard_list.h"
#include "utils.h"
#include "engine_common.h"


static const int NUM_ROWS = 10;
static const int NUM_COLUMNS = 10;


BillboardList::BillboardList ( ) :
  m_pTexture ( nullptr ),
  m_VB ( INVALID_OGL_VALUE )
{ }


BillboardList::~BillboardList ( )
{
  SafeDelete ( m_pTexture );

  if ( m_VB != INVALID_OGL_VALUE )
  {
    glDeleteBuffers ( 1, &m_VB );
  }
}

bool BillboardList::Init ( const std::string & TexFilename )
{
  m_pTexture = new Texture ( GL_TEXTURE_2D, TexFilename.c_str ( ) );

  if ( !m_pTexture->Load ( ) ) 
  {
    return false;
  }

  CreatePositionBuffer ( );

  if ( !m_technique.Init ( ) ) 
  {
    return false;
  }

  return true;
}

void BillboardList::Render ( const Matrix4f & VP, const Vector3f & cameraPos )
{
  m_technique.Enable ( );
  m_technique.SetVP ( VP );
  m_technique.SetCameraPosition ( cameraPos );

  m_pTexture->Bind ( COLOR_TEXTURE_UNIT );

  glEnableVertexAttribArray ( VertexAtribLocation::POSITIONS );

  glBindBuffer ( GL_ARRAY_BUFFER, m_VB );
  glVertexAttribPointer ( VertexAtribLocation::POSITIONS, 3, GL_FLOAT, GL_FALSE, sizeof ( Vector3f ), 0 );   // position 

  glDrawArrays ( GL_POINTS, 0, NUM_ROWS * NUM_COLUMNS );

  glDisableVertexAttribArray ( VertexAtribLocation::POSITIONS );
}

void BillboardList::CreatePositionBuffer ( )
{
  Vector3f positions[NUM_ROWS * NUM_COLUMNS];

  for ( size_t j = 0; j < NUM_ROWS; j++ ) 
  {
    for ( size_t i = 0; i < NUM_COLUMNS; i++ ) 
    {
      Vector3f pos ( ( float ) i, 0.0f, ( float ) j );
      positions[j * NUM_COLUMNS + i] = pos;
    }
  }

  glGenBuffers ( 1, &m_VB );
  glBindBuffer ( GL_ARRAY_BUFFER, m_VB );
  glBufferData ( GL_ARRAY_BUFFER, sizeof ( positions ), &positions[0], GL_STATIC_DRAW );
}
