#ifndef CALLBACKS_H
#define CALLBACKS_H

// Интерфейс для регистрации функций обратного вызова
class ICallbacks
{
public:
  virtual void SpecialKeyboardCB ( int key, int x, int y) = 0;
  virtual void KeyboardCB ( unsigned char key, int x, int y ) = 0;
  virtual void PassiveMouseCB ( int x, int y ) = 0;
  virtual void RenderSceneCB ( ) = 0;
  virtual void IdleCB ( ) = 0;
};

#endif /* CALLBACKS_H */
