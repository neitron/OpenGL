#ifndef CAMERA_H
#define	CAMERA_H

#pragma once

#include <GL\glut.h>
#include "math3d.h"
#include "Application.h"

class Camera
{
public:
  Camera() : 
    m_pos(0.0f, 0.0f, 0.0f), 
    m_target(0.0f, 0.0f, 1.0f), 
    m_up(0.0f, 1.0f, 0.0f) { };
  Camera(int wWidth, int wHeight);
  
  Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);
  
  void Camera::Init();
  void Camera::OnMouse(int x, int y);
  void Camera::OnRender();
  void Camera::Update();
  bool OnKeyboard(int Key);

  const Vector3f& GetPos() const;
  const Vector3f& GetTarget() const;
  const Vector3f& GetUp() const;
 
  static float STEP_SIZE_MOUSE; // MOUSE motion cam step
private:
  Vector3f m_pos;
  Vector3f m_target;
  Vector3f m_up;

  Vector3f m_mousePos;

  int m_windowWidth;
  int m_windowHeight;
  
  float m_AngleH;
  float m_AngleV;
  
  bool m_OnUpperEdge;
  bool m_OnLowerEdge;
  bool m_OnLeftEdge;
  bool m_OnRightEdge;

  static const float STEP_SIZE; // arrows motion cam step
  
  static const int MARGIN;  // auto rotate cam mardin screen


};

#endif /*CAMERA_H*/