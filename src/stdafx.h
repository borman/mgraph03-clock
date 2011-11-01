#ifndef STDAFX_H
#define STDAFX_H

// Standard C library
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>

// OpenGL
#include <GL/freeglut.h>

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

static inline double rand_u()
{
  return (double) rand() / (double) RAND_MAX;
}

#endif // STDAFX_H