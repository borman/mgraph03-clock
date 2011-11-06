#include "stdafx.h"
#include "loader.h"
#include "material.h"

using namespace std;

struct v3d
{
  GLfloat x, y, z;
};

struct tri
{
  int v1, v2, v3;
  int t1, t2, t3;
  int n1, n2, n3;
};

static DisplayList compile_object(const vector<v3d> &vertices, 
                                  const vector<v3d> &normals, 
                                  const vector<tri> &faces, 
                                  const Material &mtl);

static map<string, Material> mtl_load(const char *filename)
{
  map<string, Material> res;

  ifstream src;
  src.open(filename);
  if (!src.is_open())
    return res;

  string line;
  string current_mtl;
  Material mtl;
  while (!src.eof())
  {
    getline(src, line);
    char name[64];
    GLfloat r, g, b, k;

    if (strchr(line.c_str(), '#'))
      continue; // comment

    if (1 == sscanf(line.c_str(), "newmtl %s", name))
    { // object
      if (!current_mtl.empty())
      {
        cout << "finish mat " << current_mtl << ": " << mtl << endl;
        res[current_mtl] = mtl;
        mtl = Material();
      }
      current_mtl = name;
    }
    else if (3 == sscanf(line.c_str(), " Ka %f %f %f", &r, &g, &b))
      mtl.cAmbient.setRGB(r, g, b);
    else if (3 == sscanf(line.c_str(), " Kd %f %f %f", &r, &g, &b))
      mtl.cDiffuse.setRGB(r, g, b);
    else if (3 == sscanf(line.c_str(), " Ks %f %f %f", &r, &g, &b))
      mtl.cSpecular.setRGB(r, g, b);
    else if (3 == sscanf(line.c_str(), " Ke %f %f %f", &r, &g, &b))
      mtl.cEmission.setRGB(r, g, b);
    else if (1 == sscanf(line.c_str(), " Ns %f", &k))
      mtl.kShininess = k;
    else if (1 == sscanf(line.c_str(), " d %f", &k))
    {
      mtl.cDiffuse.setAlpha(k);
      mtl.cAmbient.setAlpha(k);
    }
  }
  if (!current_mtl.empty())
  {
    cout << "finish mat " << current_mtl << ": " << mtl << endl;
    res[current_mtl] = mtl;
  }

  return res;
}

NamedObjects obj_load(const char *filename)
{
  NamedObjects res;

  ifstream src;
  src.open(filename);
  if (!src.is_open())
    return res;

  map<string, Material> mtllib;
  Material mtl;
  vector<v3d> vertices;
  vector<v3d> normals;
  vector<tri> faces;

  string line;
  string current_object;
  while (!src.eof())
  {
    getline(src, line);
    char name[64];
    GLfloat x, y, z;
    int v1, v2, v3, t1, t2, t3, n1, n2, n3;

    if (strchr(line.c_str(), '#'))
      continue; // comment

    if (1 == sscanf(line.c_str(), "g %s", name))
    { // object
      if (!faces.empty())
      {
        res[current_object] = compile_object(vertices, normals, faces, mtl);
        faces.clear();
        mtl = Material();
      }
      current_object = name;
    }
    else if (1 == sscanf(line.c_str(), "mtllib %s", name))
      mtllib = mtl_load(name);
    else if (1 == sscanf(line.c_str(), "usemtl %s", name))
      mtl = mtllib[name];
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
      tri t = {
        v1-1, v2-1, v3-1, 
        0, 0, 0,
        n1-1, n2-1, n3-1
      };
      faces.push_back(t);
    }
    else if (9 == sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                         &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3))
    {
      // face
      tri t = {
        v1-1, v2-1, v3-1,
        t1-1, t2-1, t3-1,
        n1-1, n2-1, n3-1
      };
      faces.push_back(t);
    }
  }

  if (!faces.empty())
    res[current_object] = compile_object(vertices, normals, faces, mtl);

  return res;
}

static DisplayList compile_object(const vector<v3d> &vertices, const vector<v3d> &normals, const vector<tri> &faces, const Material &mtl)
{
  DisplayList l;
  l.begin();
  {
    mtl.apply();
    glBegin(GL_TRIANGLES);
      for (vector<tri>::const_iterator it = faces.begin(); it!=faces.end(); ++it)
      {
        glNormal3fv((GLfloat *) &normals[it->n1]); glVertex3fv((GLfloat *) &vertices[it->v1]);
        glNormal3fv((GLfloat *) &normals[it->n2]); glVertex3fv((GLfloat *) &vertices[it->v2]);
        glNormal3fv((GLfloat *) &normals[it->n3]); glVertex3fv((GLfloat *) &vertices[it->v3]);
      }
    glEnd();
  }
  l.end();
  return l;
}
