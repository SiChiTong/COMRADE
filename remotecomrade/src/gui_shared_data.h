#ifndef GUI_SHARED_DATA
#define GUI_SHARED_DATA

#include <GL/gl.h>
#include <math.h>
#include <vector>

const float pbo=M_PI/180.0f;

class Point
{
	public:
		Point(GLfloat x=0.0f, GLfloat y=0.0f, GLfloat z=0.0f);
		void set(GLfloat x, GLfloat y, GLfloat z);
		void setx(GLfloat x) { point[0]=x; }
		void sety(GLfloat y) { point[0]=y; }
		void setz(GLfloat z) { point[0]=z; }
		GLfloat x(void) { return(point[0]); }
		GLfloat y(void) { return(point[1]); }
		GLfloat z(void) { return(point[2]); }
	private:
		float point[3];
};

class GearData
{
	public:
		GLfloat i_rad, o_rad, width;
		unsigned short int slices;
		void set(GLfloat I_rad, GLfloat O_rad, GLfloat Width, unsigned short int Slices)
		{ i_rad = I_rad; o_rad = O_rad; width = Width; slices = Slices; }	
};

class Robot_Dimensions
{
	public:
		Robot_Dimensions();
		GLfloat chassis_length;
		GLfloat chassis_width;
		GLfloat wheel_o_rad;
		GLfloat wheel_i_rad;
		GLfloat wheel_min_width;
		GLfloat wheel_max_width;
	
		GearData gear;
		//arm details
	
};

#endif
