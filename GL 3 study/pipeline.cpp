#include "pipeline.h"

const Matrix4f& Pipeline::GetVPTrans ( )
{
  Matrix4f cameraTranslationTrans;
  Matrix4f cameraRotateTrans;
  Matrix4f persProjTrans;

  cameraTranslationTrans.InitTranslationTransform ( -m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z );
  cameraRotateTrans.InitCameraTransform ( m_camera.Target, m_camera.Up );
  persProjTrans.InitPersProjTransform ( m_persProjInfo );
  
  m_VPTtransformation = persProjTrans * cameraRotateTrans * cameraTranslationTrans;
  return m_VPTtransformation;
}

// Итоговая матрица трансформации
const Matrix4f& Pipeline::GetWVPTrans()
{
  // Матрица маштабирования
  Matrix4f ScaleTrans;
  ScaleTrans.InitScaleTransform ( m_scale.x, m_scale.y, m_scale.z );

  // Матрица поворота
  Matrix4f RotateTrans;
  RotateTrans.InitRotateTransform ( m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z );

  // Матрица Перемещения
  Matrix4f TranslationTrans;
  TranslationTrans.InitTranslationTransform ( m_worldPos.x, m_worldPos.y, m_worldPos.z );
  
  // Матрица перемещения камеры
  Matrix4f CameraTranslationTrans;
  CameraTranslationTrans.InitTranslationTransform ( -m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z );
  
  // Матрица поворота камеры
  Matrix4f CameraRotateTrans;
  CameraRotateTrans.InitCameraTransform ( m_camera.Target, m_camera.Up );
  
  // Матрица проекции перспективы камеры
  Matrix4f PersProjTrans;
  PersProjTrans.InitPersProjTransform ( m_persProjInfo );

  // Перемножаем для получения единой матрици трансформации
  m_transformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;
  
  return m_transformation;
}

const Matrix4f& Pipeline::GetWorldTrans ( )
{
  Matrix4f ScaleTrans, RotateTrans, TranslationTrans;

  ScaleTrans.InitScaleTransform ( m_scale.x, m_scale.y, m_scale.z );
  RotateTrans.InitRotateTransform ( m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z );
  TranslationTrans.InitTranslationTransform ( m_worldPos.x, m_worldPos.y, m_worldPos.z );

  m_WorldTransformation = TranslationTrans * RotateTrans * ScaleTrans;
  return m_WorldTransformation;
}