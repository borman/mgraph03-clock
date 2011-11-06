#ifndef DISPLAY_LIST_H
#define DISPLAY_LIST_H

#include "stdafx.h"

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

#endif // DISPLAY_LIST_H