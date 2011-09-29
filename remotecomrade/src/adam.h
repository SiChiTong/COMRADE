#ifndef ADAM_ROBOT_MODEL
#define ADAM_ROBOT_MODEL

#include "model.h"
#include <GL/gl.h>
#include <GL/glu.h>

class Adam : public RobotModel
{
	public:
		Adam();
		void draw(void);
	private:
		void draw_extras();
};

#endif
