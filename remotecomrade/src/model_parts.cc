#include "model_parts.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

GLuint nPartsList=0;
GLuint obstacle_list=0, wheel_list=0, gear_list=0;

void drawcube(Point endpt)
{
	glPushMatrix();
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(endpt.x(), 0.0f, 0.0f);
			glVertex3f(endpt.x(), endpt.y(), 0.0f);
			glVertex3f(0.0f, endpt.y(), 0.0f);
			glVertex3f(0.0f, endpt.y(), endpt.z());
			glVertex3f(0.0f, 0.0f, endpt.z());
			glVertex3f(endpt.x(), 0.0f, endpt.z());
			glVertex3f(endpt.x(), 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(endpt.x(), endpt.y(), endpt.z());
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, -endpt.y(), 0.0f);
			glVertex3f(-endpt.x(), -endpt.y(), 0.0f);
			glVertex3f(-endpt.x(), 0.0f, 0.0f);
			glVertex3f(-endpt.x(), 0.0f, -endpt.z());
			glVertex3f(0.0f, 0.0f, -endpt.z());
			glVertex3f(0.0f, -endpt.y(), -endpt.z());
			glVertex3f(0.0f, -endpt.y(), 0.0f);
		glEnd();	
	glPopMatrix();
}

void draw_wirecube(Point endpt)
{
	glPushMatrix();
		glBegin(GL_LINE_STRIP);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(endpt.x(), 0.0f, 0.0f);
			glVertex3f(endpt.x(), endpt.y(), 0.0f);
			glVertex3f(0.0f, endpt.y(), 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();
	
		glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, endpt.z());
	
			glVertex3f(endpt.x(), 0.0f, 0.0f);
			glVertex3f(endpt.x(), 0.0f, endpt.z());
	
			glVertex3f(endpt.x(), endpt.y(), 0.0f);
			glVertex3f(endpt.x(), endpt.y(), endpt.z());
	
			glVertex3f(0.0f, endpt.y(), 0.0f);
			glVertex3f(0.0f, endpt.y(), endpt.z());
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(endpt.x(), endpt.y(), endpt.z());
		glBegin(GL_LINE_STRIP);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, -endpt.y(), 0.0f);
			glVertex3f(-endpt.x(), -endpt.y(), 0.0f);
			glVertex3f(-endpt.x(), 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();		
	glPopMatrix();
}

void drawcube(Point corner1, Robot_Dimensions dim, Point corner2)
{
	glPushMatrix();
		glTranslatef(corner1.x(), corner1.y(), corner1.z());
		drawcube(corner2);
	glPopMatrix();
}

void draw_obstacle(Point center, GLfloat side)
{
	Point opposite(side, side, side);
	glPushMatrix();
		glTranslatef(center.x()-(side/2), center.y()-(side/2), center.z()-(side/2));
		drawcube(opposite);
	glPopMatrix();
}

void draw_obstacle(GLfloat cx, GLfloat cy, GLfloat cz, GLfloat side)
{
	/*
	Point opposite(side, side, side);
	glPushMatrix();
		glTranslatef(cx-(side/2), cy-(side/2), cz-(side/2));
		drawcube(opposite);
	glPopMatrix();
	*/
	Point opposite(1, side, 1);
	glPushMatrix();
		glTranslatef(cx-0.5, cy-(side/2), cz-0.5);
		drawcube(opposite);
	glPopMatrix();
}

void draw_wheel(GLUquadricObj *quadric, Robot_Dimensions dim, Point position)
{
	glEnable(GL_CULL_FACE);
	
	glPushMatrix();
		glColor3f(0.1f, 0.1f, 0.1f);
		glFrontFace(GL_CCW);
		glTranslatef(position.x(), position.y(), position.z()-dim.wheel_min_width/2);
		gluCylinder(quadric, dim.wheel_o_rad, dim.wheel_o_rad, dim.wheel_min_width, 32, 2);
	glPopMatrix();
	/*glPushMatrix();
		glTranslatef(position.x(), position.y(), position.z()-wheel_max_width/2);
		gluCylinder(quadric, wheel_i_rad, wheel_i_rad, wheel_max_width, 32, 2);
	glPopMatrix();*/
	//back
	glColor3f(0.7f,0.7f,0.7f);
	glPushMatrix();
		glFrontFace(GL_CW);
		glTranslatef(position.x(), position.y(), position.z()-dim.wheel_min_width/2);
		gluDisk(quadric, dim.wheel_i_rad/6, dim.wheel_i_rad, 32, 2);
	glPopMatrix();
	glColor3f(0.5f,0.5f,0.5f);
	glPushMatrix();
		glFrontFace(GL_CCW);
		glTranslatef(position.x(), position.y(), position.z()-dim.wheel_max_width/2);
		gluCylinder(quadric, (0.75*dim.wheel_o_rad)-(0.25*dim.wheel_i_rad), dim.wheel_o_rad, (dim.wheel_max_width-dim.wheel_min_width)/2, 32,2);
	glPopMatrix();
	glColor3f(0.3f,0.3f,0.3f);
	glPushMatrix();
		glFrontFace(GL_CW);
		glTranslatef(position.x(), position.y(), position.z()-dim.wheel_max_width/2);
		gluCylinder(quadric, (0.75*dim.wheel_o_rad)-(0.25*dim.wheel_i_rad), dim.wheel_i_rad, (dim.wheel_max_width-dim.wheel_min_width)/2, 32,2);
	glPopMatrix();
	glColor3f(0.1f, 0.1f, 0.1f);
	glPushMatrix();
		glTranslatef(position.x(), position.y(), position.z()-dim.wheel_max_width/2);
		gluDisk(quadric, (0.75*dim.wheel_o_rad)-(0.25*dim.wheel_i_rad), (0.75*dim.wheel_i_rad)+(0.25*dim.wheel_o_rad), 32, 2);
	glPopMatrix();
	
	//front
	glColor3f(0.7f,0.7f,0.7f);
	glFrontFace(GL_CCW);
	glPushMatrix();
		glTranslatef(position.x(), position.y(), position.z()+dim.wheel_min_width/2);
		gluDisk(quadric, dim.wheel_i_rad/6, dim.wheel_i_rad, 32, 2);
	glPopMatrix();
	glColor3f(0.5f,0.5f,0.5f);
	glPushMatrix();			
		glTranslatef(position.x(), position.y(), position.z()+dim.wheel_min_width/2);
		gluCylinder(quadric,  dim.wheel_o_rad, (0.75*dim.wheel_o_rad)-(0.25*dim.wheel_i_rad), (dim.wheel_max_width-dim.wheel_min_width)/2, 32,2);
	glPopMatrix();
	glColor3f(0.3f,0.3f,0.3f);
	glPushMatrix();
		glFrontFace(GL_CW);
		glTranslatef(position.x(), position.y(), position.z()+dim.wheel_min_width/2);
		gluCylinder(quadric, dim.wheel_i_rad, (0.75*dim.wheel_o_rad)-(0.25*dim.wheel_i_rad), (dim.wheel_max_width-dim.wheel_min_width)/2, 32,2);
	glPopMatrix();
	glColor3f(0.1f, 0.1f, 0.1f);
	glPushMatrix();			
		glFrontFace(GL_CCW);
		glTranslatef(position.x(), position.y(), position.z()+dim.wheel_max_width/2);
		gluDisk(quadric, (0.75*dim.wheel_o_rad)-(0.25*dim.wheel_i_rad), (0.75*dim.wheel_i_rad)+(0.25*dim.wheel_o_rad), 32, 2);
	glPopMatrix();
	
	glDisable(GL_CULL_FACE);
}

void draw_gear(GLUquadricObj *quadric, GearData gear, GLfloat rot)
{
	/*
	class GearData
	{
	public:
		GLfloat i_rad, o_rad, width;
		unsigned short int slices;
		void set(GLfloat I_rad, GLfloat O_rad, GLfloat Width, unsigned short int Slices)
		{ i_rad = I_rad; o_rad = O_rad; width = Width; slices = Slices; }	
	};
	*/
	
	const GLfloat i_rad = gear.i_rad;
	const GLfloat o_rad = gear.o_rad;
	const GLfloat width = gear.width;
	const unsigned short int slices = gear.slices;

	const GLfloat delta = 0.01f;
	
	glEnable(GL_CULL_FACE);
	
	//draw everything except teeth
	glColor3f(0.0f, 0.0f, 0.0f);
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, (-width/2)-delta);
		if(i_rad != 0)
		{
			glPushMatrix();
				gluCylinder(quadric, i_rad, i_rad, width+(2*delta), 32, 2);
			glPopMatrix();
		}
		glFrontFace(GL_CW);
		gluDisk(quadric, i_rad, 0.8*o_rad, 32, 2);
		glFrontFace(GL_CCW);
		gluCylinder(quadric, 0.8*o_rad, 0.8*o_rad, width+(2*delta), 32, 2);
		glTranslatef(0.0f, 0.0f, width);
		gluDisk(quadric, i_rad, 0.8*o_rad, 32, 2);
	glPopMatrix();
	
	glDisable(GL_CULL_FACE);
	
	//draw teeth;
	glColor3f(0.3f, 0.3f, 0.3f);
	glPushMatrix();
		glRotatef(rot, 0.0f, 0.0f, 1.0f);
		glPushMatrix();
		for(int i=0; i<slices; i++)
		{
			glPushMatrix();
				glTranslatef(0.0f, 0.75*o_rad, 0.0f);
				glPushMatrix();
					//draw a single tooth here
					//glTranslatef(-(0.75*o_rad)*sin(180/slices*pbo),(0.75*o_rad)*cos(180/slices*pbo), 0.0f);//-width/2);
					glBegin(GL_QUAD_STRIP);
					{
						glVertex3f(-(0.75*o_rad)*sin(180/slices*pbo), 0.0f, +width/2);
						glVertex3f(-((0.75*o_rad)*sin(180/slices*pbo))+((0.25*o_rad)*sin(180/slices*pbo)), (0.25*o_rad)*cos(180/slices*pbo), +width/2);
						glVertex3f((0.75*o_rad)*sin(180/slices*pbo), 0.0f, +width/2);
						glVertex3f(((0.75*o_rad)*sin(180/slices*pbo))-((0.25*o_rad)*sin(180/slices*pbo)), (0.25*o_rad)*cos(180/slices*pbo), +width/2);
						glVertex3f((0.75*o_rad)*sin(180/slices*pbo), 0.0f, -width/2);
						glVertex3f(((0.75*o_rad)*sin(180/slices*pbo))-((0.25*o_rad)*sin(180/slices*pbo)), (0.25*o_rad)*cos(180/slices*pbo), -width/2);
						glVertex3f(-(0.75*o_rad)*sin(180/slices*pbo), 0.0f, -width/2);
						glVertex3f(-((0.75*o_rad)*sin(180/slices*pbo))+((0.25*o_rad)*sin(180/slices*pbo)), (0.25*o_rad)*cos(180/slices*pbo), -width/2);
						glVertex3f(-(0.75*o_rad)*sin(180/slices*pbo), 0.0f, +width/2);
						glVertex3f(-((0.75*o_rad)*sin(180/slices*pbo))+((0.25*o_rad)*sin(180/slices*pbo)), (0.25*o_rad)*cos(180/slices*pbo), +width/2);
					}
					glEnd();
					glBegin(GL_QUADS);
					{
						glVertex3f(-((0.75*o_rad)*sin(180/slices*pbo))+((0.25*o_rad)*sin(180/slices*pbo)), (0.25*o_rad)*cos(180/slices*pbo), +width/2);
						glVertex3f(((0.75*o_rad)*sin(180/slices*pbo))-((0.25*o_rad)*sin(180/slices*pbo)), (0.25*o_rad)*cos(180/slices*pbo), +width/2);
						glVertex3f(((0.75*o_rad)*sin(180/slices*pbo))-((0.25*o_rad)*sin(180/slices*pbo)), (0.25*o_rad)*cos(180/slices*pbo), -width/2);
						glVertex3f(-((0.75*o_rad)*sin(180/slices*pbo))+((0.25*o_rad)*sin(180/slices*pbo)), (0.25*o_rad)*cos(180/slices*pbo), -width/2);
					}
					glEnd();
				glPopMatrix();
			glPopMatrix();
			glRotatef(360/slices, 0.0f, 0.0f, 1.0f);
		}
		glPopMatrix();
	glPopMatrix();
		

}

void draw_chain(GLUquadricObj *quadric, GearData gear, Point start, Point end, int slices)
{
	glDisable(GL_CULL_FACE);
	glColor3f(0.1f, 0.1f, 0.1f);
	GLfloat angle = 180/slices;
	glPushMatrix();
	{
		glPushMatrix();
		{
			glTranslatef(start.x(), start.y(), start.z());
			glPushMatrix();
			{
				glPushMatrix();
				{
					glTranslatef(0.0f, gear.o_rad, gear.width/2);
					Point P(sqrt((end.x()*end.x())+(end.y()*end.y())), -0.1f, -gear.width/2);
					drawcube(P);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(0.0f, -gear.o_rad+0.1f, 0.0f);
					Point P(sqrt((end.x()*end.x())+(end.y()*end.y()) ), -0.1f, -gear.width/2);
					drawcube(P);
				}
				glPopMatrix();
			}
			glPopMatrix();
			gluPartialDisk(quadric, gear.o_rad-0.1f, gear.o_rad, 8, 2, 180, 180);
			for(int i=0; i<slices; i++)
			{
				glPushMatrix();
					glTranslatef(0.0f, gear.o_rad, 0.0f);
					
					glPushMatrix();
						//draw a single tooth here
						//glTranslatef(-(0.75*o_rad)*sin(180/slices*pbo),(0.75*o_rad)*cos(180/slices*pbo), 0.0f);//-width/2);
						glBegin(GL_QUADS);
						{
							glVertex3f(-gear.o_rad*angle*pbo/2, 0.0f, gear.width/2);
							glVertex3f(-gear.o_rad*angle*pbo/2, 0.0f, -gear.width/2);
							glVertex3f(gear.o_rad*angle*pbo/2, 0.0f, -gear.width/2);
							glVertex3f(gear.o_rad*angle*pbo/2, 0.0f, gear.width/2);
						}
						glEnd();
					glPopMatrix();
				glPopMatrix();
				glRotatef(angle, 0.0f, 0.0f, 1.0f);
			}
		}
		glPopMatrix();
		
		glPushMatrix();
		{
			glTranslatef(sqrt( ((end.x()-start.x())*(end.x()-start.x())) +( (end.y()-start.y())*(end.y()-start.y()) ) ), 0.0f, 0.0f);
			glRotatef(180+angle, 0.0f, 0.0f, 1.0f);
			gluPartialDisk(quadric, gear.o_rad-0.1f, gear.o_rad, 8, 2, 180, 180);
			for(int i=0; i<slices; i++)
			{
				glPushMatrix();
					glTranslatef(0.0f, gear.o_rad, 0.0f);
					glPushMatrix();
						//draw a single tooth here
						//glTranslatef(-(0.75*o_rad)*sin(180/slices*pbo),(0.75*o_rad)*cos(180/slices*pbo), 0.0f);//-width/2);
						glBegin(GL_QUADS);
						{
							glVertex3f(-gear.o_rad*angle*pbo/2, 0.0f, gear.width/2);
							glVertex3f(-gear.o_rad*angle*pbo/2, 0.0f, -gear.width/2);
							glVertex3f(gear.o_rad*angle*pbo/2, 0.0f, -gear.width/2);
							glVertex3f(gear.o_rad*angle*pbo/2, 0.0f, gear.width/2);
						}
						glEnd();
					glPopMatrix();
				glPopMatrix();
				glRotatef(angle, 0.0f, 0.0f, 1.0f);
			}
		}
		glPopMatrix();
	}	
	glPopMatrix();
}

void init_displaylist(void)
{
	nPartsList = glGenLists(3);
}

void render_obstacle(GLfloat cx, GLfloat cy, GLfloat cz, GLfloat side)
{
	obstacle_list = nPartsList;
	glNewList(obstacle_list, GL_COMPILE_AND_EXECUTE);
		draw_obstacle(cx, cy, cz, side);
	glEndList();
}

void render_wheel(GLUquadricObj *quadric, Robot_Dimensions dim, Point position)
{
	wheel_list = nPartsList+1;
	glNewList(wheel_list, GL_COMPILE_AND_EXECUTE);
		draw_wheel(quadric, dim, position);
	glEndList();
}

void render_gear(GLUquadricObj *quadric, GearData gear, GLfloat rot)
{
	gear_list = nPartsList+2;
	glNewList(gear_list, GL_COMPILE_AND_EXECUTE);
		//draw_gear(quadric, gear, rot);
	glEndList();
}

void draw_rendered_obstacle()
{	glCallList(obstacle_list);	}

void draw_rendered_wheel()
{	glCallList(wheel_list);	}

void draw_rendered_gear()
{	glCallList(gear_list);	}

void cleanup_displaylist(void)
{
	glDeleteLists(nPartsList, 3);
}
