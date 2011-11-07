#include "stdafx.h"
#include "scene.h"
#include "loader.h"
#include "gl_utils.h"
#include "texture.h"

static DisplayList objSpheres;
static DisplayList objGround;
static NamedObjects objsClock;

static void prepare_objects(void);
static void draw_pancake(double radius, int n_sectors);
static void draw_objects(bool needGround);
static void draw_clock(GLfloat hour, GLfloat min, GLfloat sec);

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
  GLfloat light1_color[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light2_color[] = { 0.0, 10.0, 0.0, 1.0 };

  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_color);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);

  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1_color);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.01);
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1);

  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_color);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2_color);
  glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 1);
  glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 2.0);

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Color(0.1, 0.1, 0.1).toGL());
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);

  // Default material
  glShadeModel(GL_SMOOTH);

  glEnable(GL_NORMALIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_TEXTURE_2D);
  prepare_objects();
}

void scene_display(bool needReflection)
{
  glInitNames();
  // Reflection
  if (needReflection)
  {
    glPushMatrix();
      glScalef(1, 1, -1);
      glFrontFace(GL_CW);

      draw_objects(false);

      glFrontFace(GL_CCW);
    glPopMatrix();  
  }

  // Real objects
  draw_objects(true);
}

void scene_interact(void)
{
  static int mode = 0;
  GLfloat colors[][4] = {
    { 0.0, 10.0, 0.0, 1.0 },
    { 10.0, 0.0, 0.0, 1.0 }
  };

  mode = 1-mode;
  glLightfv(GL_LIGHT2, GL_DIFFUSE, colors[mode]);
  glLightfv(GL_LIGHT2, GL_SPECULAR, colors[mode]);
}

// ----------------

static void prepare_objects(void)
{
  // Create spheres
  objSpheres.begin();
  {
    Material mat(clWhite, 0.3, clWhite, Color(0.05, 0.02, 0.08, 0.01));
    mat.apply(); // FIXME: something's wrong over there
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    static const int N_SPHERES = 10;
    for (int i=0; i<N_SPHERES; i++)
    {
      GLdouble  dist = 17 + (rand_u()-0.5) * 10;
      GLdouble  phi = rand_u() * 4 * M_PI;

      GLdouble x = dist*cos(phi);
      GLdouble y = dist*sin(phi);
      GLdouble z = 1*rand_u();
      GLdouble r = 1 + dist*0.2*rand_u();

      glColor4d(0.5 + 0.2*(phi<=M_PI? sin(phi) : 0),
                0.2 + 0.2*(abs(cos(phi))),
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
    glEnable(GL_TEXTURE_2D);
    Texture tex;
    if (LoadTGA(&tex, "vortex.tga")) 
    { 
      glGenTextures(1, &tex.texID);
      glBindTexture(GL_TEXTURE_2D, tex.texID);
      glTexImage2D(GL_TEXTURE_2D, 0, tex.bpp / 8, tex.width, tex.height, 0, tex.type, GL_UNSIGNED_BYTE, tex.imageData);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      free(tex.imageData);
      tex.imageData = 0;
    }

    Material mat(Color(1.0, 1.0, 1.0, 0.7), 1.0);
    mat.apply(); 
    draw_pancake(50, 20);
    glDisable(GL_TEXTURE_2D);
  }
  objGround.end();

  objsClock = obj_load("clock_meshes.obj");
}

static void draw_pancake(double radius, int n_sectors)
{
  static const GLfloat normal[] = {0.0, 0.0, 1.0};
  glBegin(GL_TRIANGLE_FAN);
  {
    static const GLdouble texScale = 0.8;
    glNormal3fv(normal);
    glTexCoord2d(0.5, 0.5);
    glVertex3d(0, 0, 0);
    for (int i=0; i<=n_sectors; i++)
    {
      double phi = i*2*M_PI/n_sectors;
      glNormal3fv(normal);
      glTexCoord2d(0.5 + cos(phi)/texScale, 0.5 + sin(phi)/texScale);
      glVertex3d(radius*cos(phi), radius*sin(phi), 0);
    }
  }
  glEnd();
}

//=======================================

static void draw_objects(bool needGround)
{
  static GLdouble clock_radius = 3.0;
  static GLdouble clock_height = 0.2;

  // Get clock time
  struct timeb time;
  ftime(&time);

  GLfloat sec = (time.time%60) + (time.millitm/1000.0);
  GLfloat min = (time.time/60 + time.timezone) % 60;
  GLfloat hour = (((time.time/60 - time.timezone)/60) + (time.dstflag?1:0)) % 12;

  // Place lights
  GLfloat light0[] = {1.0, 1.0, 20.0, 1.0};
  GLfloat light1[] = {0.0, 2.5, 4.0,  1.0};
  GLfloat light2[] = {0.0, 0.2, clock_height+clock_radius,  1.0};
  {
    GLdouble phi = (sec/60 + 0.25) * 2*M_PI;
    light2[0] += cos(phi)*2.3;
    light2[2] += sin(phi)*2.3;
  }

  glLightfv(GL_LIGHT0, GL_POSITION, light0);
  glLightfv(GL_LIGHT1, GL_POSITION, light1);
  glLightfv(GL_LIGHT2, GL_POSITION, light2);

  // Ground pancake
  if (needGround)
    objGround.exec();

  GLdouble ground_plane[] = {0.0, 0.0, 1.0, 0.0};
  glEnable(GL_CLIP_PLANE0);
  {
    glClipPlane(GL_CLIP_PLANE0, ground_plane);
    objSpheres.exec();
  
    glPushMatrix();
      glTranslated(0.0, 0.0, clock_height+clock_radius);
      glScaled(clock_radius, clock_radius, clock_radius);
      draw_clock(hour, min, sec);
    glPopMatrix();
  }
  glDisable(GL_CLIP_PLANE0);
}

static void draw_clock(GLfloat hour, GLfloat min, GLfloat sec)
{
  GLfloat tstamp = sec + 60*min + 3600*hour;

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

  glPushName(1);
  objsClock["Clock"].exec();
  glPopName();
  
  glPushMatrix();
    glRotatef(-6*sec, 0, 1, 0);
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
