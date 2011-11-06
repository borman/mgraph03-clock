#include "stdafx.h"
#include "scene.h"
#include "loader.h"
#include "display_list.h"
#include "material.h"

static DisplayList objSpheres;
static DisplayList objGround;
static NamedObjects objsClock;

static void prepare_objects(void);
static void draw_pancake(double radius, int n_sectors);
static void draw_objects(void);
static void draw_clock(void);

void scene_init(void)
{
  glEnable(GL_FOG);
  {
    GLfloat fogColor[4] = {0.0, 0.0, 0.0, 1.0};
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.06);
    glHint(GL_FOG_HINT, GL_NICEST);
    glFogf(GL_FOG_START, 10.0);
    glFogf(GL_FOG_END, 30.0);
  }
  
  // Lighting
  GLfloat light0_color[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light1_color[] = { 2.0, 2.0, 2.0, 1.0 };
  GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_color);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);

  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1_color);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.01);
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1);

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Color(0.1, 0.1, 0.1).toGL());
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  // Default material
  glShadeModel(GL_SMOOTH);

  glEnable(GL_NORMALIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  prepare_objects();
}

void scene_display(void)
{
  // Position lights
  GLfloat light0[] = {1.0, 1.0, 20.0, 1.0};
  GLfloat light1[] = {0.0, 2.0, 4, 1.0};

  GLdouble ground_plane[] = {0.0, 0.0, 1.0, 0.0};

  // Reflection
  glPushMatrix();
    glEnable(GL_CLIP_PLANE0);
    glScalef(1, 1, -1);
    glClipPlane(GL_CLIP_PLANE0, ground_plane);
    glLightfv(GL_LIGHT0, GL_POSITION, light0);
    glLightfv(GL_LIGHT1, GL_POSITION, light1);
    glFrontFace(GL_CW);

    draw_objects();

    glFrontFace(GL_CCW);
    glDisable(GL_CLIP_PLANE0);
  glPopMatrix();

  glLightfv(GL_LIGHT0, GL_POSITION, light0);
  glLightfv(GL_LIGHT1, GL_POSITION, light1);

  // Ground pancake
  objGround.exec();

  // Real objects
  glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, ground_plane);
    draw_objects();
  glDisable(GL_CLIP_PLANE0);
}

// ----------------

static void prepare_objects(void)
{
    // Create spheres
  objSpheres.begin();
  {
    Material mat(clWhite, 0.3, clWhite, Color(0.1, 0.1, 0.1, 0.01));
    mat.apply(); // FIXME: something's wrong over there
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    static const int N_SPHERES = 10;
    for (int i=0; i<N_SPHERES; i++)
    {
      GLdouble  dist = 13 + (rand_u()-0.5) * 10;
      GLdouble  phi = rand_u() * 4 * M_PI;

      GLdouble x = dist*cos(phi);
      GLdouble y = dist*sin(phi);
      GLdouble z = 1*rand_u();
      GLdouble r = 1 + dist*0.2*rand_u();

      glColor4d(0.8 + 0.2*(phi<=M_PI? sin(phi) : 0),
                0.8 + 0.2*(abs(cos(phi))),
                0.8 + 0.2*(phi>=M_PI? -sin(phi) : 0),
                1.0);
      glPushMatrix();
        glTranslated(x, y, z);
        glutSolidSphere(r, 20, 20);
      glPopMatrix();
    }

    glDisable(GL_COLOR_MATERIAL);
  }
  objSpheres.end();
  
  objGround.begin();
  {
    Material mat(Color(0.4, 0.1, 0.5, 0.7), 1.0);
    mat.apply(); 
    draw_pancake(50, 20);
  }
  objGround.end();

  objsClock = obj_load("clock_meshes.obj");
}

static void draw_pancake(double radius, int n_sectors)
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

static void draw_objects(void)
{
  objSpheres.exec();
  
  glPushMatrix();
    glTranslated(0.0, 0.0, 3.1);
    glScaled(3.0, 3.0, 3.0);
    draw_clock();
  glPopMatrix();
}

static void draw_clock(void)
{
  GLfloat clock_color[4] = {1.0, 1.0, 1.0, 0.7};
  GLfloat handle_color[4] = {0.3, 0.3, 0.3, 1.0};

  struct timeb time;
  ftime(&time);

  GLfloat sec = (time.time%60) + (time.millitm/1000.0);
  GLfloat min = (time.time/60 + time.timezone) % 60;
  GLfloat hour = (((time.time/60 - time.timezone)/60) + (time.dstflag?1:0)) % 12;

  GLfloat tstamp = sec + 60*min + 3600*hour;

  Material mat_clock(Color(1.0, 1.0, 1.0, 0.7), 200.0, clWhite);
  Material mat_handle(Color(0.3, 0.3, 0.3, 1.0));

  mat_clock.apply();

  glPushMatrix();
  {
    GLdouble k = 0.1*sin(tstamp * 2 * M_PI);
    glTranslated(k, 0, 0);
    objsClock["Holder1"].exec();
    glTranslated(-2*k, 0, 0);
    objsClock["Holder2"].exec();
  }
  glPopMatrix();

  glPushMatrix();
  {
    GLdouble k = 1.0 + -abs(0.3*sin(tstamp * 2 * M_PI) - 0.1);
    glScaled(k, k, k);
    objsClock["Heart"].exec();
  }
  glPopMatrix();

  objsClock["Clock"].exec();
  
  mat_handle.apply();

  glPushMatrix();
    glRotatef(- 6*sec, 0, 1, 0);
    objsClock["SecondHandle"].exec();
  glPopMatrix();

  glPushMatrix();
    glRotatef(-6*min, 0, 1, 0);
    objsClock["MinuteHandle"].exec();
  glPopMatrix();

  glPushMatrix();
    glRotatef(-30*hour, 0, 1, 0);
    objsClock["HourHandle"].exec();
  glPopMatrix();
}
