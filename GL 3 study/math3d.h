#ifndef MATH_3D_H
#define	MATH_3D_H

const float M_PI = 3.141592653f; // 3.141592653 58979323846


// Угол (алгебраический)
class Angle
{
public:
  
  // Радианы в градусы
  inline static float ToDegree(float x)
  {
    return (x * 180.0f / M_PI);
  };
  
  // Градусы в радианы
  inline static float ToRadian(float x)
  {
    return (x * M_PI / 180.0f);
  };

private:
  
  Angle() { };
  ~Angle() { };
};



struct Vector2i
{
  int x;
  int y;
};



struct Vector2f
{
  float x;
  float y;
  
  Vector2f() { }
  Vector2f(float _x, float _y) : 
    x(_x), 
    y(_y) { }
};



// Вектор 3D :float
struct Vector3f
{
  float x;
  float y;
  float z;
  
  Vector3f() { }
  Vector3f ( float _x ) :
    x ( _x ),
    y ( _x ),
    z ( _x ) { }
  Vector3f(float _x, float _y, float _z) : 
    x(_x), 
    y(_y), 
    z(_z) { }
  Vector3f(const Vector3f &v);
  
  Vector3f Cross(const Vector3f &v) const;
  Vector3f& Normalize();
  
  Vector3f  operator * (const float f) const;
  Vector3f& operator = (const Vector3f &v);
  Vector3f& operator += (const Vector3f &v);
  Vector3f& operator -= (const Vector3f &v);
  Vector3f& operator *= (const float f);
  
  friend inline Vector3f operator+( const Vector3f& l, const Vector3f& r )
  {
    Vector3f Ret ( l.x + r.x,
      l.y + r.y,
      l.z + r.z );

    return Ret;
  }

  friend inline Vector3f operator-( const Vector3f& l, const Vector3f& r )
  {
    Vector3f Ret ( l.x - r.x,
      l.y - r.y,
      l.z - r.z );

    return Ret;
  }

  void Rotate ( float angle, const Vector3f& axis );
};

// Матрица 4х4 :float
class Matrix4f
{
public:
  float m[4][4];

  Matrix4f() { }

  // Единичная матрица
  inline void InitIdentity()
  {
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
  }
  
  inline Matrix4f operator*(const Matrix4f& right) const
  {
    Matrix4f Ret;

    for (unsigned int i = 0 ; i < 4 ; i++) {
      for (unsigned int j = 0 ; j < 4 ; j++) {
        Ret.m[i][j] = m[i][0] * right.m[0][j] +
          m[i][1] * right.m[1][j] +
          m[i][2] * right.m[2][j] +
          m[i][3] * right.m[3][j];
      }
    }
    return Ret;
  }

  void InitScaleTransform(float scaleX, float scaleY, float scaleZ);
  void InitRotateTransform(float rotateX, float rotateY, float rotateZ);
  void InitTranslationTransform(float x, float y, float z);
  void InitCameraTransform(const Vector3f& target, const Vector3f& up);
  void InitPersProjTransform(float FOV, float width, float height, float zNear, float zFar);
};

// Кватернион
class Quaternion
{
public:
  
  float x;
  float y;
  float z;
  float w;
 
  Quaternion() { }
  Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }
  Quaternion(const Quaternion &v);

  void Quaternion::Normalize();
  Quaternion Quaternion::Conjugate() const;
  friend Quaternion operator*(const Quaternion& l, const Quaternion& r);
  friend Quaternion operator*(const Quaternion& q, const Vector3f& v);
};

#endif	/* MATH_3D_H */