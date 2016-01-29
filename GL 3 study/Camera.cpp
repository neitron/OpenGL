#pragma once

#include <math.h>

#include <GL\glut.h>

#include "Camera.h"


const float Camera::STEP_SIZE ( 0.1f );  // arrows motion cam step

const int Camera::MARGIN ( 10 ); // auto rotate cam mardin screen

float Camera::STEP_SIZE_MOUSE ( 0.1f );  // MOUSE motion cam step


Camera::Camera ( int wWidth, int wHeight ) :
  m_pos           ( Vector3f ( 0.0f, 0.0f, 0.0f ) ),
  m_target        ( Vector3f ( 0.0f, 0.0f, 1.0f ) ),
  m_up            ( Vector3f ( 0.0f, 1.0f, 0.0f ) ),
  m_windowWidth   ( wWidth ),
  m_windowHeight  ( wHeight )
{
  m_target.Normalize ( );

  Init ( );
};

Camera::Camera ( int wWidth, int wHeight, const Vector3f& pos, const Vector3f& target, const Vector3f& up ) :
  m_pos           ( pos ),
  m_target        ( target ),
  m_up            ( up ),
  m_windowWidth   ( wWidth ),
  m_windowHeight  ( wHeight )
{
  m_target.Normalize ( );
  m_up.Normalize ( );

  Init ( );
}

void Camera::Init ( )
{
  Vector3f hTarget ( m_target.x, 0.0, m_target.z );
  hTarget.Normalize ( );

  if ( hTarget.z >= 0.0f )
  {
    if ( hTarget.x >= 0.0f )
    {
      m_AngleH = 360.0f - Angle::ToDegree ( asin ( hTarget.z ) );
    }
    else
    {
      m_AngleH = 180.0f + Angle::ToDegree ( asin ( hTarget.z ) );
    }
  }
  else
  {
    if ( hTarget.x >= 0.0f )
    {
      m_AngleH = Angle::ToDegree ( asin ( -hTarget.z ) );
    }
    else
    {
      m_AngleH = 90.0f + Angle::ToDegree ( asin ( -hTarget.z ) );
    }
  }

  m_AngleV = -Angle::ToDegree ( asin ( m_target.y ) );

  m_mousePos.x = m_windowWidth / 2;
  m_mousePos.y = m_windowHeight / 2;

  glutWarpPointer ( m_mousePos.x, m_mousePos.y );
}

void Camera::OnMouse ( int x, int y )
{
  if ( ( x == m_mousePos.x ) && ( y == m_mousePos.y ) ) 
    return;

  const int DeltaX = x - m_mousePos.x;
  const int DeltaY = y - m_mousePos.y;

  m_AngleH += static_cast< float >( DeltaX ) * STEP_SIZE_MOUSE;
  m_AngleV += static_cast< float >( DeltaY ) * STEP_SIZE_MOUSE;

  Update ( );
  glutWarpPointer ( m_mousePos.x, m_mousePos.y );
}

void Camera::OnRender ( )
{
  bool ShouldUpdate = false;

  if ( ShouldUpdate )
  {
    Update ( );
  }
}

void Camera::Update ( )
{
  const Vector3f vaxis ( 0.0f, 1.0f, 0.0f );

  // Rotate the view vector by the horizontal angle around the vertical axis
  Vector3f View ( 1.0f, 0.0f, 0.0f );
  View.Rotate ( m_AngleH, vaxis );
  View.Normalize ( );

  // Rotate the view vector by the vertical angle around the horizontal axis
  Vector3f haxis = vaxis.Cross ( View );
  haxis.Normalize ( );
  View.Rotate ( m_AngleV, haxis );

  m_target = View;
  m_target.Normalize ( );

  m_up = m_target.Cross ( haxis );
  m_up.Normalize ( );
}

bool Camera::OnKeyboard ( int Key )
{
  bool isKeyPresed = false;

  switch ( Key )
  {
  case GLUT_KEY_UP:
  {
    m_pos += ( m_target * STEP_SIZE );
    isKeyPresed = true;
  }
  break;

  case GLUT_KEY_DOWN:
  {
    m_pos -= ( m_target * STEP_SIZE );
    isKeyPresed = true;
  }
  break;

  case GLUT_KEY_LEFT:
  {
    Vector3f Left = m_target.Cross ( m_up );
    Left.Normalize ( );
    Left *= STEP_SIZE;
    m_pos += Left;
    isKeyPresed = true;
  }
  break;

  case GLUT_KEY_RIGHT:
  {
    Vector3f Right = m_up.Cross ( m_target );
    Right.Normalize ( );
    Right *= STEP_SIZE;
    m_pos += Right;
    isKeyPresed = true;
  }
  break;

  case GLUT_KEY_END:
  {
    m_pos = Vector3f ( 0.0f, 0.0f, 0.0f );
    m_target = Vector3f ( 0.0f, 0.0f, 1.0f );
    m_target.Normalize ( );
    m_up = Vector3f ( 0.0f, 1.0f, 0.0f );
    Init ( );
  }
  break;

  default:
  {

  }
  }

  return isKeyPresed;
}

const Vector3f& Camera::GetPos ( ) const
{
  return m_pos;
}

const Vector3f& Camera::GetTarget ( ) const
{
  return m_target;
}

const Vector3f& Camera::GetUp ( ) const
{
  return m_up;
}