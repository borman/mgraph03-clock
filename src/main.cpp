#include "stdafx.h"
#include "scene.h"
#include "camera.h"

/* GLUT callback Handlers */

static GLfloat aspectRatio = 1;

static void on_resize(int width, int height)
{
  aspectRatio = (float) width / (float) height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50, aspectRatio, 0.1, 1000);

  glMatrixMode(GL_MODELVIEW);
}

static void on_display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera_setup();
  scene_display(true);

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
  static const GLdouble u_power = 0.001;
  static const GLdouble v_power = 0.001;
  switch (key)
  {
  case GLUT_KEY_LEFT:  camera_impulse(-u_power, 0); break;
  case GLUT_KEY_RIGHT: camera_impulse(u_power, 0); break;
  case GLUT_KEY_UP:    camera_impulse(0, v_power); break;
  case GLUT_KEY_DOWN:  camera_impulse(0, -v_power); break;
  }
}

static int drag_base_x = 0;
static int drag_base_y = 0;
static bool drag_active = false;

static void do_drag(int x, int y)
{
  GLdouble nx = -0.005 * ((GLdouble) (x - drag_base_x));
  GLdouble ny = 0.005 * ((GLdouble) (y - drag_base_y));
  drag_base_x = x;
  drag_base_y = y;
  camera_drag(nx, ny);
}

static void do_select(int x, int y)
{
  static const int hitBufSize = 100;
  GLuint hits[hitBufSize];
  glSelectBuffer(hitBufSize, hits);

  glRenderMode(GL_SELECT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluPickMatrix(x, viewport[3]-y, 3, 3, viewport);
	gluPerspective(50, aspectRatio, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);

  camera_setup();
  scene_display(false);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	
	int nHits = glRenderMode(GL_RENDER);
  if (nHits > 0)
  {
    // we've hit something...
    bool ok = false;
    int p = 0;
    while (nHits > 0)
    {
      GLuint nNames = hits[p];
      if (nNames > 0) // ...and it has a name
      {
        ok = true;
        break;
      }

      p += 3 + nNames; // nNames, mindepth, maxdepth
      nHits--;
    }
    if (ok) 
      scene_interact();
  }
}

static void on_mouse(int button, int state, int x, int y)
{
  if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
  {
    camera_drag_start();
    drag_base_x = x;
    drag_base_y = y;
    drag_active = true;
  }

  if (button==GLUT_LEFT_BUTTON && state==GLUT_UP)
  {
    camera_drag_end();
    drag_active = false;
  }

  if (button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    do_select(x, y);

  glutPostRedisplay();
}

static void on_motion(int x, int y)
{
  if (drag_active)
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
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

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

#ifdef WIN32
int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  int argc=1;
  char *argv[] = {"clock", 0};
  return main(argc, argv);
}
#endif // WIN32