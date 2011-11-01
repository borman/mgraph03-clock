#include "stdafx.h"
#include "scene.h"

struct sphere
{
  double x;
  double y;
  double z;
  double r;
};

#define N_SPHERES 10
static struct sphere spheres[N_SPHERES];

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
    spheres[i].r = 0.5 + 2*rand_u();
  }

  glEnable(GL_FOG);
  {
    GLfloat fogColor[4] = {0.0, 0.0, 0.0, 1.0};
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.07);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
  }
}

void scene_display(void)
{
  glBegin(GL_TRIANGLE_FAN);
  int n_sectors = 20;
  double radius = 30;
  glColor3d(0.5, 0.5, 0.5);
  glVertex3d(0, 0, 0);
  for (int i=0; i<=n_sectors; i++)
  {
    double phi = i*2*M_PI/n_sectors;
    glColor3d(phi<=M_PI? sin(phi) : 0, abs(cos(phi)), phi>=M_PI? -sin(phi) : 0);
    glVertex3d(radius*cos(phi), radius*sin(phi), 0);
  }
  glEnd();

    for (int i=0; i<N_SPHERES; i++)
    {
      glPushMatrix();
      glTranslated(spheres[i].x, spheres[i].y, spheres[i].z);
      glutSolidSphere(spheres[i].r, 20, 20);
      glPopMatrix();
    }

}
