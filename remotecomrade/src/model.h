#ifndef ROBOT_MODEL
#define ROBOT_MODEL

#include "gui_shared_data.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
//#include <fltk/GlWindow.h>
//#include <fltk/events.h>

class RobotModel
{
	protected:
		GLUquadricObj *quadric;
		Robot_Dimensions dim;
		Point pos, dir;

		void drawbasic(void);
		virtual void draw_extras(void);
		void draw_chassis(void);
		void draw_body(void);
		void draw_drive(void);

	public:
		RobotModel();
		virtual ~RobotModel();
		void draw();
		void move(Point position, Point direction);
};

/*
class Eve : public RobotModel
{
	
};

class Adam : public RobotModel
{
	
};
*/
#endif
