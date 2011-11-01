#include "stdafx.h"
#include "scene.h"

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


static void objPancake(double radius, int n_sectors);

void scene_init(void)
{
  // Create spheres
  for (int i=0; i<N_SPHERES; i++)
  {
    double r = 3 + rand_u() * 20;
    double phi = rand_u() * 2 * M_PI;
    spheres[i].x = r*cos(phi);
    spheres[i].y = r*sin(phi);
    spheres[i].z = 0;
    spheres[i].r = 0.5 + r*0.2*rand_u();
    spheres[i].color[0] = phi<=M_PI? sin(phi) : 0;
    spheres[i].color[1] = abs(cos(phi));
    spheres[i].color[2] = phi>=M_PI? -sin(phi) : 0;
  }

  glEnable(GL_FOG);
  {
    GLfloat fogColor[4] = {0.0, 0.0, 0.0, 1.0};
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.06);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
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
  GLfloat light0[] = { 1.0, 1.0, 5.0, 0.0 };
  GLfloat light1[] = { -4.0, -4.0, 1.0, 0.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light0);
  glLightfv(GL_LIGHT1, GL_POSITION, light1);

  // Ground pancake
  static const GLfloat pancake_color[] = {1.0, 1.0, 1.0, 1.0};
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pancake_color);
  objPancake(30, 20);

  // Spheres
  for (int i=0; i<N_SPHERES; i++)
  {
    glPushMatrix();
    glTranslated(spheres[i].x, spheres[i].y, spheres[i].z);
    //glColor3dv(spheres[i].color);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, spheres[i].color);
    glutSolidSphere(spheres[i].r, 20, 20);
    glPopMatrix();
  }
}

// ----------------

static void objPancake(double radius, int n_sectors)
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