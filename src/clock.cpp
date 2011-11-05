#include "stdafx.h"

using namespace std;

struct v3d
{
  GLfloat x, y, z;
};

struct tri
{
  int v1, v2, v3;
  int n1, n2, n3;
};

vector<v3d> vertices;
vector<v3d> normals;
vector<vector<tri> > objects;
map<string, int> obj_names;

static void load_mtl(const char *file)
{
}

void clock_load(void)
{
  ifstream src;
  src.open("clock_meshes.obj");
  if (!src.is_open())
    return;

  string line;
  int obj = 0;
  while (!src.eof())
  {
    getline(src, line);
    char objname[64];
    GLfloat x, y, z;
    int v1, v2, v3, n1, n2, n3;

    if (strchr(line.c_str(), '#'))
      continue; // comment

    if (1 == sscanf(line.c_str(), "g %s", objname))
    { // object
      objects.push_back(vector<tri>());
      obj = objects.size()-1;
      obj_names[objname] = obj;
      cout << "obj " << objname << endl;
    }
    else if (3 == sscanf(line.c_str(), "v %f %f %f", &x, &y, &z))
    {
      // vertex
      v3d v = {x, y, z};
      vertices.push_back(v);
    }
    else if (3 == sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z))
    {
      // vertex normal
      v3d v = {x, y, z};
      normals.push_back(v);
    }
    else if (6 == sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3))
    {
      // face
      tri t = {v1-1, v2-1, v3-1, n1-1, n2-1, n3-1};
      objects[obj].push_back(t);
    }
  }
}

static void draw_object(int index)
{
  const vector<tri> &tris = objects[index];
  glBegin(GL_TRIANGLES);
  for (vector<tri>::const_iterator it = tris.begin(); it!=tris.end(); ++it)
  {
    glNormal3fv((GLfloat *) &normals[it->n1]); glVertex3fv((GLfloat *) &vertices[it->v1]);
    glNormal3fv((GLfloat *) &normals[it->n2]); glVertex3fv((GLfloat *) &vertices[it->v2]);
    glNormal3fv((GLfloat *) &normals[it->n3]); glVertex3fv((GLfloat *) &vertices[it->v3]);
  }
  glEnd();
}

void clock_draw(void)
{
  GLfloat clock_color[4] = {1.0, 1.0, 1.0, 0.2};
  GLfloat handle_color[4] = {0.3, 0.3, 0.3, 1.0};

  struct timeb time;
  ftime(&time);

  GLfloat sec = (time.time%60) + (time.millitm/1000.0);
  GLfloat min = (time.time/60 + time.timezone) % 60;
  GLfloat hour = (((time.time/60 - time.timezone)/60) + (time.dstflag?1:0)) % 12;

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, clock_color);

  draw_object(obj_names["Clock"]);
  
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, handle_color);

  glPushMatrix();
  glRotatef(- 6*sec, 0, 1, 0);
  draw_object(obj_names["SecondHandle"]);
  glPopMatrix();

  glPushMatrix();
  glRotatef(-6*min, 0, 1, 0);
  draw_object(obj_names["MinuteHandle"]);
  glPopMatrix();

  glPushMatrix();
  glRotatef(-30*hour, 0, 1, 0);
  draw_object(obj_names["HourHandle"]);
  glPopMatrix();
}