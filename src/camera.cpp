#include "stdafx.h"
#include "camera.h"

/** Inertial movement
  
  x' = v0 * exp(-kt)
  x = x0 + (v0/k)(1-exp(-kt))
**/
static const GLdouble friction = 0.002;

static GLdouble u_speed = 0;
static GLdouble v_speed = 0;
static GLdouble u_base = M_PI/2;
static GLdouble v_base = 0.3;
static GLdouble u_offset = 0;
static GLdouble v_offset = 0;

static int time_offset = 0;
static bool is_dragged = false;

static GLdouble get_pos(GLdouble base, GLdouble offset, GLdouble speed, int time)
{
  if (is_dragged)
    return base + offset;
  else
    return base + (speed / friction) * (1 - exp(-friction * time));
}

static GLdouble get_speed(GLdouble speed, int time)
{
  if (is_dragged)
    return speed;
  else
    return speed * exp(-friction * time);
}

static void fix(void)
{
  int time = glutGet(GLUT_ELAPSED_TIME);
  int dtime = time - time_offset;

  // Fix current state
  u_base = get_pos(u_base, u_offset, u_speed, dtime);
  v_base = get_pos(v_base, v_offset, v_speed, dtime);
  u_speed = get_speed(u_speed, dtime);
  v_speed = get_speed(v_speed, dtime);
  time_offset = time;
}

void camera_setup(void)
{
  // Setup camera
  int time = glutGet(GLUT_ELAPSED_TIME) - time_offset;

  GLdouble u = get_pos(u_base, u_offset, u_speed, time);
  GLdouble v = get_pos(v_base, v_offset, v_speed, time);

  static const GLdouble v_low = 0.1;
  static const GLdouble v_high = M_PI/2 - 0.1;
  if (v < v_low)
  {
    v_base = v = v_low;
    v_speed = 0;
    v_offset = 0;
  }
  if (v > v_high)
  {
    v_base = v = v_high;
    v_speed = 0;
    v_offset = 0;
  }

  double rad = 10;
  double camx = rad * cos(u) * cos(v);
  double camy = rad * sin(u) * cos(v);
  double camz = rad * sin(v);

  glLoadIdentity() ;
  gluLookAt(camx, camy, camz, 
            0,    0.0, 3.0, 
            0,    0,    1);
}

void camera_impulse(GLdouble du, GLdouble dv)
{
  fix();
  u_speed += du;
  v_speed += dv;
}

void camera_drag_start(void)
{
  fix();

  is_dragged = true;
  u_offset = 0;
  v_offset = 0;
}

void camera_drag_end(void)
{
  if (glutGet(GLUT_ELAPSED_TIME) - time_offset > 100)
  {
    // full stop
    u_speed = 0;
    v_speed = 0;
  }

  fix();

  is_dragged = false;
}

void camera_drag(GLdouble du, GLdouble dv)
{
  int time = glutGet(GLUT_ELAPSED_TIME);
  int dtime = time - time_offset;

  u_offset += du;
  v_offset += dv;
  u_speed = du/dtime;
  v_speed = dv/dtime;
  time_offset = time;
}