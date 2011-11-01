#include "stdafx.h"
#include "scene.h"
#include "camera.h"

/* GLUT callback Handlers */

static void
  resize(int width, int height)
{
  const float ar = (float) width / (float) height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
}

static void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera_setup();
  scene_display();

  glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
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

static void special (int key, int x, int y)
{
}


static void idle(void)
{
  glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
  glutInitWindowSize(640,480);
  glutInitWindowPosition(40,40);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

  glutCreateWindow("OpenGLUT Shapes");

  glutReshapeFunc(resize);
  glutDisplayFunc(display);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutIdleFunc(idle);

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