#include <GL/gl.h>
#include "gui_shared_data.h"

Point::Point(GLfloat x, GLfloat y, GLfloat z) {
	point[0]=x;
	point[1]=y;
	point[2]=z;
}

void Point::set(GLfloat x, GLfloat y, GLfloat z) {
	point[0]=x;
	point[1]=y;
	point[2]=z;
}

Robot_Dimensions::Robot_Dimensions()
{
	/*
	chassis_length = 12.0f;
	chassis_width = 10.0f;
	wheel_i_rad = 0.8f;
	wheel_o_rad = 1.6f;
	wheel_min_width = 1.7f;
	wheel_max_width = 2.0f;
	*/
	chassis_length = 24.0f;
	chassis_width = 20.0f;
	wheel_i_rad = 1.6f;
	wheel_o_rad = 3.2f;
	wheel_min_width = 3.4f;
	wheel_max_width = 4.0f;
	
	gear.set(wheel_i_rad/6, 1.0f, 0.2f, 16);
	//arm details
	
}
