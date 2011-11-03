#include "stdafx.h"
#include "scene.h"
#include "obj_parser/obj_parser.h"

struct sphere
{
  double x;
  double y;
  double z;
  double r;
  GLfloat color[3];
};

#define N_SPHERES 10
static struct sphere spheres[N_SPHERES];


static void drawPancake(double radius, int n_sectors);
static void drawObjects(void);

static void loadModel(void);

void scene_init(void)
{
  // Create spheres
  for (int i=0; i<N_SPHERES; i++)
  {
    double r = 3 + rand_u() * 20;
    double phi = rand_u() * 2 * M_PI;
    spheres[i].x = r*cos(phi);
    spheres[i].y = r*sin(phi);
    spheres[i].z = 5*rand_u();
    spheres[i].r = 0.5 + r*0.2*rand_u();
    spheres[i].color[0] = phi<=M_PI? sin(phi) : 0;
    spheres[i].color[1] = abs(cos(phi));
    spheres[i].color[2] = phi>=M_PI? -sin(phi) : 0;
  }

  glEnable(GL_FOG);
  {
    GLfloat fogColor[4] = {0.0, 0.0, 0.0, 1.0};
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.06);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 15.0);
    glFogf(GL_FOG_END, 50.0);
  }
  
  // Lighting
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  // Default material
  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 80.0 };
  glShadeModel(GL_SMOOTH);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void scene_display(void)
{
  // Position lights
  GLfloat light0[] = {1.0, 1.0, 5.0, 0.0};
  GLfloat light1[] = {-4.0, -4.0, 1.0, 0.0};

  GLdouble ground_plane[] = {0.0, 0.0, 1.0, 0.0};

  // Reflection
  glPushMatrix();
    glEnable(GL_CLIP_PLANE0);
    glScalef(1, 1, -1);
    glClipPlane(GL_CLIP_PLANE0, ground_plane);
    glLightfv(GL_LIGHT0, GL_POSITION, light0);
    glLightfv(GL_LIGHT1, GL_POSITION, light1);
    glFrontFace(GL_CW);

    drawObjects();

    glFrontFace(GL_CCW);
    glDisable(GL_CLIP_PLANE0);
  glPopMatrix();

  glLightfv(GL_LIGHT0, GL_POSITION, light0);
  glLightfv(GL_LIGHT1, GL_POSITION, light1);

  // Ground pancake
  glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_ONE, GL_ONE);
    static const GLfloat pancake_color[] = {1.0, 1.0, 1.0, 0.7};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pancake_color);
    drawPancake(50, 20);
  glDisable(GL_BLEND);

  // Real objects
  glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, ground_plane);
    drawObjects();
  glDisable(GL_CLIP_PLANE0);
}

// ----------------

static void drawPancake(double radius, int n_sectors)
{
  static const GLfloat normal[] = {0.0, 0.0, 1.0};
  glBegin(GL_TRIANGLE_FAN);
  {
    glNormal3fv(normal);
    glVertex3d(0, 0, 0);
    for (int i=0; i<=n_sectors; i++)
    {
      double phi = i*2*M_PI/n_sectors;
      glNormal3fv(normal);
      glVertex3d(radius*cos(phi), radius*sin(phi), 0);
    }
  }
  glEnd();
}

static void drawObjects(void)
{
  for (int i=0; i<N_SPHERES; i++)
  {
    glPushMatrix();
    glTranslated(spheres[i].x, spheres[i].y, spheres[i].z);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, spheres[i].color);
    glutWireSphere(spheres[i].r, 20, 20);
    glPopMatrix();
  }
}

static void loadModel(void)
{
  obj_scene_data data;
  if( !parse_obj_scene(&data, "test.obj") )
    return;

  delete_obj_data(&data);
}