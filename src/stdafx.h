#ifndef STDAFX_H
#define STDAFX_H

// Standard C library
#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstring>
#include <sys/types.h>
#include <sys/timeb.h>

// Standard C++ library
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <fstream>


// OpenGL
#include <GL/freeglut.h>

// VC++ stdlib fix
#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

// Common definitions
static inline double rand_u()
{
  return (double) rand() / (double) RAND_MAX;
}

#endif // STDAFX_H
