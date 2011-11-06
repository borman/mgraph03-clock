#ifndef MATERIAL_H
#define MATERIAL_H

#include "stdafx.h"

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

static inline std::ostream &operator<<(std::ostream &s, const Color &c)
{
  return s << "RGBA(" << c.r << "," << c.g << "," << c.b << "," << c.a << ")";
}

static const Color clWhite(1.0, 1.0, 1.0);
static const Color clBlack(0.0, 0.0, 0.0);

struct Material
{
  Material(const Color &cBase=clWhite, GLfloat kShin=0.0, const Color &cSpec=clBlack, const Color &cEmi=clBlack)
    : cAmbient(cBase*0.2), cDiffuse(cBase), cSpecular(cSpec), cEmission(cEmi), kShininess(kShin) {}

  void apply() const;

  Color cAmbient, cDiffuse, cSpecular, cEmission;
  GLfloat kShininess;
};

static inline std::ostream &operator<<(std::ostream &s, const Material &m)
{
  return s << "MTL(A=" << m.cAmbient << ",D=" << m.cDiffuse << ",S=" << m.cSpecular << ",kS=" << m.kShininess << ")";
}

#endif // MATERIAL_H