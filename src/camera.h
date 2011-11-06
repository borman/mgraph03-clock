#ifndef CAMERA_H
#define CAMERA_H

void camera_setup(void);

void camera_drag_start(void);
void camera_drag_end(void);
void camera_drag(GLdouble du, GLdouble dv);

void camera_impulse(GLdouble du, GLdouble dv);

#endif // CAMERA_H