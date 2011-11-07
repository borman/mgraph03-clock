#ifndef CLOCK_H
#define CLOCK_H

#include "stdafx.h"
#include "gl_utils.h"

typedef std::map<std::string, DisplayList> NamedObjects;

NamedObjects obj_load(const char *filename);

#endif // CLOCK_H
