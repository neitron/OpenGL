#ifndef CAMERA_H
#define	CAMERA_H

#pragma once

#include "math3d.h"


class Camera
{
public:
  Camera() : 
    m_pos(0.0f, 0.0f, 0.0f), 
    m_target(0.0f, 0.0f, 1.0f), 
    m_up(0.0f, 1.0f, 0.0f) { };
  Camera(int wWidth, int wHeight);
  
  Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);
  
  void OnMouse(int x, int y);
  void OnRender();
  bool OnKeyboard(int key);

  const Vector3f& GetPos()    const;
  const Vector3f& GetTarget() const;
  const Vector3f& GetUp()     const;

private:

  void Init ( );
  void Update ( );

  Vector3f m_pos;
  Vector3f m_target;
  Vector3f m_up;

  Vector2i m_mousePos;

  int m_windowWidth;
  int m_windowHeight;
  
  float m_AngleH;
  float m_AngleV;

  static float STEP_SIZE_MOUSE; // MOUSE motion cam step

  static const float STEP_SIZE; // arrows motion cam step
  
  static const int MARGIN;      // auto rotate cam mardin screen
};

#endif /*CAMERA_H*/