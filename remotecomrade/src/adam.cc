#include "adam.h"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "model_parts.h"

Adam::Adam() : RobotModel()
{
	
}

void Adam::draw_extras()
{
	//we draw a cpu for now. we'll leave the camera for later
	const GLfloat t=sqrtf(((dim.chassis_length/4*tan(30*pbo))*(dim.chassis_length/4*tan(30*pbo)))+((dim.wheel_o_rad+1)*(dim.wheel_o_rad+1)));
	const GLfloat tx = t*cos(60*pbo)*cos(30*pbo);
	const GLfloat ty = t*sin(60*pbo);
	const GLfloat tz = t*cos(60*pbo)*sin(30*pbo);
	Point opp_vertex((dim.chassis_length/2)-tx, ty/2, -dim.chassis_width+(2*tz));
	glPushMatrix();
	{
		glTranslatef(0.0f, ty, (dim.chassis_width/2)-tz);
		glColor3f(0.3f, 0.3f, 0.3f);
		drawcube(opp_vertex);
		glPushMatrix();
		{
			glTranslatef((dim.chassis_length/2)-tx, ty/3, -0.75*dim.chassis_width);
			glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
			glColor3f(0.06f, 0.06f, 0.06f);
			gluCylinder(quadric, 0.1f, 0.08f, ty/8, 20, 2);
			gluCylinder(quadric, 0.08f, 0.08f, ty/2, 20, 2);
			glTranslatef(0.0f, 0.0f, ty/2);
			gluSphere(quadric, 0.08f, 20, 20);
		}
		glPopMatrix();
	}
	glPopMatrix();
	
}

void Adam::draw(void)
{
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	glPushMatrix();
		drawbasic();
		draw_extras();
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glFlush();
}
