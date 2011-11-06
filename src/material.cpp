#include "material.h"

void Material::apply() const
{
  glMaterialfv(GL_FRONT, GL_AMBIENT, cAmbient.toGL());
  glMaterialfv(GL_FRONT, GL_DIFFUSE, cDiffuse.toGL());
  glMaterialf(GL_FRONT, GL_SHININESS, kShininess);
  glMaterialfv(GL_FRONT, GL_SPECULAR, cSpecular.toGL());
  glMaterialfv(GL_FRONT, GL_EMISSION, cEmission.toGL());
}