#ifndef UTILS_H
#define UTILS_H

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

const unsigned int INVALID_UNIFORM_LOCATION = 0xFFFFFFFF;

const unsigned int INVALID_OGL_VALUE = 0xFFFFFFFF;

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#endif /* UTILS_H */