#ifndef MODEL_PARTS
#define MODEL_PARTS

#include "gui_shared_data.h"
#include <GL/glu.h>

void drawcube(Point endpt);
void draw_wirecube(Point endpt);
void drawcube(Point corner1, Point corner2);
void draw_obstacle(Point center, GLfloat side);
void draw_obstacle(GLfloat cx, GLfloat cy, GLfloat cz, GLfloat side);
void draw_wheel(GLUquadricObj *quadric, Robot_Dimensions dim, Point position);
void draw_gear(GLUquadricObj *quadric, GearData gear, GLfloat rot);
void draw_chain(GLUquadricObj *quadric, GearData gear, Point start, Point end, int slices);

void init_displaylist(void);
void render_obstacle(GLfloat cx, GLfloat cy, GLfloat cz, GLfloat side);
void render_wheel(GLUquadricObj *quadric, Robot_Dimensions dim, Point position);
void render_gear(GLUquadricObj *quadric, GearData gear, GLfloat rot);
void draw_rendered_obstacle(void);
void draw_rendered_wheel(void);
void draw_rendered_gear(void);
void cleanup_displaylist(void);

#endif
