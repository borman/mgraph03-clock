#include "stdafx.h"
#include "scene.h"
#include "camera.h"

/* GLUT callback Handlers */

static void on_resize(int width, int height)
{
  const float ar = (float) width / (float) height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
}

static void on_display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera_setup();
  scene_display();

  glutSwapBuffers();
}


static void on_key(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27 :
  case 'Q':
  case 'q': glutLeaveMainLoop () ;      break;

  default:
    break;
  }

  glutPostRedisplay();
}

static void on_special (int key, int x, int y)
{
}

static int drag_base_x = 0;
static int drag_base_y = 0;

static void do_drag(int x, int y)
{
  GLdouble nx = -0.005 * ((GLdouble) (x - drag_base_x));
  GLdouble ny = 0.005 * ((GLdouble) (y - drag_base_y));
  drag_base_x = x;
  drag_base_y = y;
  camera_drag(nx, ny);
}

static void on_mouse(int button, int state, int x, int y)
{
  if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
  {
    camera_drag_start();
    drag_base_x = x;
    drag_base_y = y;
  }

  if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
  {
    camera_drag_end();
  }
  glutPostRedisplay();
}

static void on_motion(int x, int y)
{
  do_drag(x, y);
  glutPostRedisplay();
}

static void on_idle(void)
{
  glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
  glutInitWindowSize(640,480);
  glutInitWindowPosition(40,40);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

  glutCreateWindow("OpenGL Clock");

  glutReshapeFunc(on_resize);
  glutDisplayFunc(on_display);
  glutKeyboardFunc(on_key);
  glutMouseFunc(on_mouse);
  glutMotionFunc(on_motion);
  glutSpecialFunc(on_special);
  glutIdleFunc(on_idle);

  glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION ) ;

  glClearColor(0, 0, 0, 0);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  scene_init();

  glutMainLoop();

  return EXIT_SUCCESS;
}