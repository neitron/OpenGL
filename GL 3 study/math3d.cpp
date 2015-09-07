#include "math3d.h"

Vector3f::Vector3f(const Vector3f &v)
{
  x = v.x;
  y = v.y;
  z = v.z;
}

// Возвращает вектор, перпендикулярный плоскости, 
// определяемой исходными векторами
Vector3f Vector3f::Cross(const Vector3f& v) const
{
  Vector3f temp(
    y * v.z - z * v.y,
    z * v.x - x * v.z,
    x * v.y - y * v.x);

    return temp;
}

// Нормализация вектора (что бы его длина равна была 1)
Vector3f& Vector3f::Normalize()
{
    const float Length = sqrtf(x * x + y * y + z * z);

    x /= Length;
    y /= Length;
    z /= Length;

    return *this;
}

Vector3f& Vector3f::operator += (const Vector3f &v)
{
  x += v.x;
  y += v.y;
  z += v.z;

  return *this;
}
Vector3f& Vector3f::operator -= (const Vector3f &v)
{
  x -= v.x;
  y -= v.y;
  z -= v.z;

  return *this;
}
Vector3f& Vector3f::operator *= (const float f)
{
  x *= f;
  y *= f;
  z *= f;

  return *this;
}
Vector3f& Vector3f::operator =  (const Vector3f &v)
{
  x = v.x;
  y = v.y;
  z = v.z;

  return *this;
}
Vector3f  Vector3f::operator *  (const float f) const
{
  Vector3f temp(
    x * f,
    y * f,
    z * f);

  return temp;
}

// Вращение вектора вокрух оси axis на угол angle (кватернионы)
void Vector3f::Rotate(float angle, const Vector3f& axis)
{
  const float SinHalfAngle = sinf(Angle::ToRadian(angle/2));
  const float CosHalfAngle = cosf(Angle::ToRadian(angle/2));
  
  const float Rx = axis.x * SinHalfAngle;
  const float Ry = axis.y * SinHalfAngle;
  const float Rz = axis.z * SinHalfAngle;
  const float Rw = CosHalfAngle;
  Quaternion RotationQ(Rx, Ry, Rz, Rw);
  
  Quaternion ConjugateQ = RotationQ.Conjugate();
    ConjugateQ.Normalize();
  Quaternion W = RotationQ * (*this) * ConjugateQ;
  
  x = W.x;
  y = W.y;
  z = W.z;
}

// Матрица преобразования маштабирования
void Matrix4f::InitScaleTransform(float scaleX, float scaleY, float scaleZ)
{
    m[0][0] = scaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
    m[1][0] = 0.0f;   m[1][1] = scaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = scaleZ; m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
}

// Матрица преобразования вращения
void Matrix4f::InitRotateTransform(float rotateX, float rotateY, float rotateZ)
{
    Matrix4f rx, ry, rz;

    const float x = Angle::ToRadian(rotateX);
    const float y = Angle::ToRadian(rotateY);
    const float z = Angle::ToRadian(rotateZ);

    rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f;     rx.m[0][2] = 0.0f;      rx.m[0][3] = 0.0f;
    rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x);  rx.m[1][2] = -sinf(x);  rx.m[1][3] = 0.0f;
    rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x);  rx.m[2][2] = cosf(x);   rx.m[2][3] = 0.0f;
    rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f;     rx.m[3][2] = 0.0f;      rx.m[3][3] = 1.0f;

    ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y); ry.m[0][3] = 0.0f;
    ry.m[1][0] = 0.0f;    ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f;     ry.m[1][3] = 0.0f;
    ry.m[2][0] = sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y);  ry.m[2][3] = 0.0f;
    ry.m[3][0] = 0.0f;    ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f;     ry.m[3][3] = 1.0f;

    rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z);  rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
    rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z);   rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
    rz.m[2][0] = 0.0f;    rz.m[2][1] = 0.0f;      rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
    rz.m[3][0] = 0.0f;    rz.m[3][1] = 0.0f;      rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

    *this = rz * ry * rx;
}

// Матрица преобразования переноса
void Matrix4f::InitTranslationTransform(float x, float y, float z)
{
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

// Матрица преобразования камеры
// (проекции на плоскость камеры)
void Matrix4f::InitCameraTransform(const Vector3f& target, const Vector3f& up)
{
    Vector3f N = target;
    N.Normalize();
    Vector3f U = up;
    U.Normalize();
    U = U.Cross(N);
    Vector3f V = N.Cross(U);
       
    m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
    m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
    m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
    m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;        
}

// Матрица преобразования перспективы
void Matrix4f::InitPersProjTransform(float FOV, float width, float height, float zNear, float zFar)
{
    const float ar         = width / height;
    const float zRange     = zNear - zFar;
    const float tanHalfFOV = tanf(Angle::ToRadian(FOV / 2.0f));

    m[0][0] = 1.0f/(tanHalfFOV * ar); m[0][1] = 0.0f;            m[0][2] = 0.0f;          m[0][3] = 0.0;
    m[1][0] = 0.0f;                   m[1][1] = 1.0f/tanHalfFOV; m[1][2] = 0.0f;          m[1][3] = 0.0;
    m[2][0] = 0.0f;                   m[2][1] = 0.0f;            m[2][2] = (-zNear -zFar)/zRange ; m[2][3] = 2.0f * zFar*zNear/zRange;
    m[3][0] = 0.0f;                   m[3][1] = 0.0f;            m[3][2] = 1.0f;          m[3][3] = 0.0;
}

Quaternion::Quaternion(const Quaternion &v)
{
  x = v.x;
  y = v.y;
  z = v.z;
  w = v.w;
}

// Нормировать кватернион
void Quaternion::Normalize()
{
    float Length = sqrtf(x * x + y * y + z * z + w * w);

    x /= Length;
    y /= Length;
    z /= Length;
    w /= Length;
}

// Смежный кватернион
Quaternion Quaternion::Conjugate() const
{
    Quaternion ret(-x, -y, -z, w);
    return ret;
}

// Умножение кватернионов
Quaternion operator*(const Quaternion& l, const Quaternion& r)
{
    const float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
    const float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
    const float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
    const float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

    Quaternion ret(x, y, z, w);

    return ret;
}

// Умножуние кватерниона на вектор
Quaternion operator*(const Quaternion& q, const Vector3f& v)
{
    const float w = - (q.x * v.x) - (q.y * v.y) - (q.z * v.z);
    const float x =   (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
    const float y =   (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
    const float z =   (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

    Quaternion ret(x, y, z, w);

    return ret;
}
