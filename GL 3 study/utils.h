#ifndef UTILS_H
#define UTILS_H

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0])) // depricate
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; } // depricate


const unsigned int INVALID_UNIFORM_LOCATION = 0xFFFFFFFF;
const unsigned int INVALID_OGL_VALUE        = 0xFFFFFFFF;


template <class T>
// Array size (in elements)
unsigned int Arraylength ( T& a )
{
  return 
    sizeof ( a ) / sizeof ( a[0] );
}


template <class T> 
// Safe point deleting
void SafeDelete ( T& p )
{
  if ( p )
  {
    delete p;
    p = 0;
  }
}


#endif /* UTILS_H */