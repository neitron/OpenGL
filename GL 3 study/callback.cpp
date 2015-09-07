//#pragma once
//
//#include "callback.h"
//
//// Set call back func
//void CallBack::InitializeGlutCallbacks()
//{
//
//
//  Vector3f Vertices[1];
//  Vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);
//
//  // Set func for render 1 frame (~ DF ~)
//  glutDisplayFunc(RenderSceneCB);
//}
//
//void CallBack::RenderSceneCB()
//{
//  glClear(GL_COLOR_BUFFER_BIT);
//
//  glBegin(GL_TRIANGLES);
//  {
//    glColor3f(1.0, 0.0, 0.0);
//    glVertex3f(0.0, 0.5, 0.0);
//
//    glColor3f(0.0, 1.0, 0.0);
//    glVertex3f(0.5, -0.5, 0.0);
//
//    glColor3f(0.0, 0.0, 1.0);
//    glVertex3f(-0.5, -0.5, 0.0);
//  }
//  glEnd();
//
//  // Change buffes
//  glutSwapBuffers();
//}