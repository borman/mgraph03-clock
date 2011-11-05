#include "stdafx.h"
#include "camera.h"

/** Inertial movement
  
  x' = v0 * exp(-kt)
  x = x0 + (v0/k)(1-exp(-kt))
**/
static const GLdouble friction = 0.001;

static GLdouble u_speed = 0;
static GLdouble v_speed = 0;
static GLdouble u_base = M_PI/2;
static GLdouble v_base = 0.3;

static int time_offset = 0;

static GLdouble get_pos(GLdouble base, GLdouble speed, int time)
{
  return base + (speed / friction) * (1 - exp(-friction * time));
}

static GLdouble get_speed(GLdouble speed, int time)
{
  return speed * exp(-friction * time);
}

static GLdouble clamp(GLdouble low, GLdouble v, GLdouble high)
{
  if (v > high)
    return high;
  if (v < low)
    return low;
  return v;
}

void camera_setup(void)
{
  // Setup camera
  int time = glutGet(GLUT_ELAPSED_TIME) - time_offset;

  GLdouble u = get_pos(u_base, u_speed, time);
  GLdouble v = get_pos(v_base, v_speed, time);

  static const GLdouble v_low = 0.1;
  static const GLdouble v_high = M_PI/2 - 0.1;
  if (v < v_low)
  {
    v_base = v = v_low;
    v_speed = 0;
  }
  if (v > v_high)
  {
    v_base = v = v_high;
    v_speed = 0;
  }

  double rad = 10;
  double camx = rad * cos(u) * cos(v);
  double camy = rad * sin(u) * cos(v);
  double camz = rad * sin(v);

  glLoadIdentity() ;
  gluLookAt(camx, camy, camz, 0, 0, 2.0, 0, 0, 1);
}

void camera_impulse(GLdouble du, GLdouble dv)
{
  int time = glutGet(GLUT_ELAPSED_TIME);
  int dtime = time - time_offset;

  // Fix current state
  u_base = get_pos(u_base, u_speed, dtime);
  v_base = get_pos(v_base, v_speed, dtime);
  u_speed = get_speed(u_speed, dtime) + du;
  v_speed = get_speed(v_speed, dtime) + dv;
  time_offset = time;
}