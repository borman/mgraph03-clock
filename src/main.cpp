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

static void on_mouse(int button, int state, int x, int y)
{
  GLdouble nx = -0.001 * ((GLdouble) x / (GLdouble) glutGet(GLUT_WINDOW_WIDTH) - 0.5);
  GLdouble ny = -0.001 * ((GLdouble) y / (GLdouble) glutGet(GLUT_WINDOW_HEIGHT) - 0.5);
  camera_impulse(nx, ny);
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

  glutCreateWindow("OpenGLUT Shapes");

  glutReshapeFunc(on_resize);
  glutDisplayFunc(on_display);
  glutKeyboardFunc(on_key);
  glutMouseFunc(on_mouse);
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