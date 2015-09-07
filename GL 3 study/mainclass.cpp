//#pragma once
//
//#include "mainclass.h"
//
//
//int Main::main(int _argc, char** _argv)
//{
//  // Init GLUT
//  glutInit(&_argc, _argv);
//
//  // Options GLUT
//  // GLUT_DOUBLE - double bufferization
//  // GLUT_RBA - buffer of color
//  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
//
//  // You know
//  glutInitWindowSize(1024, 768);
//  glutInitWindowPosition(0, 0);
//  glutCreateWindow("Tutorial 01");
//
//  // Init Glut call back
//  CallBack::InitializeGlutCallbacks();
//
//  // Must be done after glut is initialized!
//  GLenum res = glewInit();
//  if (res != GLEW_OK) {
//    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
//    return 1;
//  }
//
//  // Set state for GL_COLOR_BUFFER_BIT
//  glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
//
//  // Get control GLUT
//  /*В этом цикле он ждёт событий от оконной 
//  системы и передаёт их через функции обратного 
//  вызова, которые мы задали ранее.*/
//  glutMainLoop();
//
//  return 0;
//}