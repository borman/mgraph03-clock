#ifndef GL_UTILS_H
#define GL_UTILS_H

#include "stdafx.h"


// ==============
class DisplayList
{
public:
  DisplayList() : m_isValid(false), m_id(0) {}

  void begin()
  {
    m_isValid = true;
    m_id = glGenLists(1);
    glNewList(m_id, GL_COMPILE);
  }
  void end() { glEndList(); }

  bool isValid() const { return m_isValid; }
  void exec() const { if (isValid()) glCallList(m_id); }
private:

  bool m_isValid;
  GLuint m_id;
};


// ==============
struct Color
{
  Color(GLfloat _r=0.0, GLfloat _g=0.0, 
        GLfloat _b=0.0, GLfloat _a=1.0)
        : r(_r), g(_g), b(_b), a(_a) {};

  Color operator*(GLfloat k) const { return Color(r*k, g*k, b*k, a); }

  void setRGB(GLfloat _r, GLfloat _g, GLfloat _b) { r=_r; g=_g; b=_b; }
  void setAlpha(GLfloat _a) { a=_a; }

  // zomg ugly hack
  const GLfloat *toGL() const { return &r; } 

  GLfloat r, g, b, a;
};

static const Color clWhite(1.0, 1.0, 1.0);
static const Color clBlack(0.0, 0.0, 0.0);


// ==============
struct Material
{
  explicit Material(const Color &cBase=clWhite, GLfloat kShin=0.0, const Color &cSpec=clBlack, const Color &cEmi=clBlack)
    : cAmbient(cBase*0.2), cDiffuse(cBase), cSpecular(cSpec), cEmission(cEmi), kShininess(kShin) {}

  void apply() const
  {
    glMaterialfv(GL_FRONT, GL_AMBIENT, cAmbient.toGL());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cDiffuse.toGL());
    glMaterialf(GL_FRONT, GL_SHININESS, kShininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, cSpecular.toGL());
    glMaterialfv(GL_FRONT, GL_EMISSION, cEmission.toGL());
  }

  Color cAmbient, cDiffuse, cSpecular, cEmission;
  GLfloat kShininess;
};


// ==============
static inline std::ostream &operator<<(std::ostream &s, const Color &c)
{
  return s << "RGBA(" << c.r << "," << c.g << "," << c.b << "," << c.a << ")";
}

static inline std::ostream &operator<<(std::ostream &s, const Material &m)
{
  return s << "MTL(A=" << m.cAmbient << ",D=" << m.cDiffuse << ",S=" << m.cSpecular << ",kS=" << m.kShininess << ")";
}

#endif // GL_UTILS_H