#include "stdafx.h"
#include "camera.h"

void camera_setup(void)
{
  // Setup camera
  double phi = glutGet(GLUT_ELAPSED_TIME) * (M_PI/10000);
  double rad = 10;
  double camx = rad*cos(phi);
  double camy = rad*sin(phi);
  double camz = 4 + 2*sin(phi*3.5);

  glLoadIdentity() ;
  gluLookAt(camx, camy, camz, 0, 0, 2.0, 0, 0, 1);
}
