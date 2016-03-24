#ifndef PIPELINE_H
#define	PIPELINE_H

#include "math3d.h"

class Pipeline
{
public:

    Pipeline()
    {
        m_scale      = Vector3f(1.0f, 1.0f, 1.0f);
        m_worldPos   = Vector3f(0.0f, 0.0f, 0.0f);
        m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        m_scale.x = ScaleX;
        m_scale.y = ScaleY;
        m_scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
        m_worldPos.x = x;
        m_worldPos.y = y;
        m_worldPos.z = z;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
      m_rotateInfo.x = RotateX;
      m_rotateInfo.y = RotateY;
      m_rotateInfo.z = RotateZ;
    }

    void SetPerspectiveProj( PersProjInfo& p )
    {
      m_persProjInfo.FOV    = p.FOV;
      m_persProjInfo.width  = p.width;
      m_persProjInfo.height = p.height;
      m_persProjInfo.zNear  = p.zNear;
      m_persProjInfo.zFar   = p.zFar;
    }

    void SetCamera ( const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up )
    {
      m_camera.Pos    = Pos;
      m_camera.Target = Target;
      m_camera.Up     = Up;
    }

    const Matrix4f & GetVPTrans ( );

    // Итоговая матрица трансформации
    const Matrix4f& GetWVPTrans ( );
    const Matrix4f& GetWorldTrans ( );

private:

    Vector3f m_scale;
    Vector3f m_worldPos;
    Vector3f m_rotateInfo;

    PersProjInfo m_persProjInfo;

    struct 
    {
        Vector3f Pos;
        Vector3f Target;
        Vector3f Up;
    } m_camera;

    Matrix4f m_transformation;
    Matrix4f m_WorldTransformation;
    Matrix4f m_VPTtransformation;
};


#endif	/* PIPELINE_H */