#include "model.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include "model_parts.h"

RobotModel::RobotModel() {
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	/*
	init_displaylist();
	render_gear(quadric, dim.gear, 0);
	render_obstacle(-0.5, 5, -0.5, 10);
	Point pos;
	render_wheel(quadric, dim, pos);
	*/
}

RobotModel::~RobotModel() {
	gluDeleteQuadric(quadric);
	//cleanup_displaylist();
}

void RobotModel::draw(void)
{
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	glPushMatrix();
		drawbasic();
		//draw_extras();
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glFlush();
}

void RobotModel::drawbasic(void)
{
	glPushMatrix();
		draw_chassis();
		draw_drive();
		draw_body();
	glPopMatrix();
}

void RobotModel::draw_extras(void) { std::cerr << "calling RobotModel::draw_exras(), are you sure you want to call this???"; }

void RobotModel::draw_chassis(void)
{
	Point current;
	Point opp_vertex;
	glColor3f(0.4f, 0.4, 0.5f);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glPushMatrix();	
		glTranslatef(-dim.chassis_length/2, -0.50f, dim.chassis_width/2);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		opp_vertex.set(1,1,dim.chassis_length);
		drawcube(opp_vertex);
		glPushMatrix();
			glTranslatef(1.6*dim.wheel_max_width, 0.0f, 0.0f);
			drawcube(opp_vertex);
		glPopMatrix();		
		glTranslatef(dim.chassis_width-1, 0.0f, 0.0f);
		//opp_vertex.set(1,1,chassis_length);
		drawcube(opp_vertex);
		glPushMatrix();
			glTranslatef(-1.6*dim.wheel_max_width, 0.0f, 0.0f);
			drawcube(opp_vertex);
		glPopMatrix();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-dim.chassis_length/2, -0.50f, -dim.chassis_width/2);
		opp_vertex.set(1,1,dim.chassis_width);
		drawcube(opp_vertex);
		glTranslatef(dim.chassis_length-1, 0.0f, 0.0f);
		drawcube(opp_vertex);
	glPopMatrix();
	glFrontFace(GL_CCW);
	//battery
	glPushMatrix();
	{
		glPushMatrix();
		{
			
			glTranslatef(-dim.chassis_length/12, 0.50f, (dim.chassis_width/2)-(1.6*dim.wheel_max_width));
			glColor3f(0.6f, 0.6f, 1.0f);
			opp_vertex.set(dim.chassis_length/6, 1.5f, -dim.chassis_width+(3.2*dim.wheel_max_width));
			
			drawcube(opp_vertex);
		}
		glPopMatrix();
		glPushMatrix();
		{
			GLfloat delta=(dim.chassis_width-(3.2f*dim.wheel_max_width))/6;
			glColor3f(0.4f, 0.4f, 1.0f);
			glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
			
			glTranslatef(0.0f, (dim.chassis_width/2)-(1.6f*dim.wheel_max_width)+(delta/2), 2.0f);
			for(int i=0; i<6; i++)
			{
				glTranslatef(0.0f, -delta, 0.0f);
				gluCylinder(quadric, 0.1f, 0.1f, 0.2f, 20, 2);
				glPushMatrix();
				{
					glTranslatef(0.0f, 0.0f, 0.2f);
					gluDisk(quadric, 0.0f, 0.1f, 20,1);
				}
				glPopMatrix();
			}
		}
		glPopMatrix();
	}
	glPopMatrix();

	
	
	//Draw axles
	glColor3f(0.2f,0.2f,0.2f);
	glPushMatrix();
		glTranslatef(-dim.chassis_length/2 +dim.chassis_length/4, 0.0f, -dim.chassis_width/2 - 1.5);
		gluCylinder(quadric, dim.wheel_i_rad/6, dim.wheel_i_rad/6, dim.chassis_width+3, 32, 2);
	glPopMatrix();
	
	glPushMatrix();
		glColor3f(1.0f,1.0f,1.0f);
		glTranslatef(-dim.chassis_length/2 +dim.chassis_length/4, 0.0f, dim.chassis_width/2 + 1.5);
		gluDisk(quadric, 0.0f, dim.wheel_i_rad/6, 32, 1);
	glPopMatrix();
	
	glPushMatrix();
		glFrontFace(GL_CW);
		glTranslatef(-dim.chassis_length/2 +dim.chassis_length/4, 0.0f, -dim.chassis_width/2 - 1.5);
		gluDisk(quadric, 0.0f, dim.wheel_i_rad/6, 32, 1);
	glPopMatrix();
	
	
	glPushMatrix();
		glFrontFace(GL_CCW);
		glColor3f(0.2f, 0.2f, 0.2f);
		glTranslatef(dim.chassis_length/2 -dim.chassis_length/4, 0.0f, -dim.chassis_width/2 - 1.5);
		gluCylinder(quadric, dim.wheel_i_rad/6, dim.wheel_i_rad/6, dim.chassis_width+3, 32, 2);
	glPopMatrix();
	
	glPushMatrix();
		glColor3f(1.0f,1.0f,1.0f);
		glTranslatef(dim.chassis_length/2 -dim.chassis_length/4, 0.0f, dim.chassis_width/2 + 1.5);
		gluDisk(quadric, 0.0f, dim.wheel_i_rad/6, 32, 1);
	glPopMatrix();
	
	glPushMatrix();
		glFrontFace(GL_CW);
		glTranslatef(dim.chassis_length/2 -dim.chassis_length/4, 0.0f, -dim.chassis_width/2 - 1.5);
		gluDisk(quadric, 0.0f, dim.wheel_i_rad/6, 32, 1);
	glPopMatrix();
	
	glFrontFace(GL_CCW);
	
	glDisable(GL_CULL_FACE);
	
	//Draw Wheels
	glPushMatrix();
		current.set(-dim.chassis_length/2 + dim.chassis_length/4, 0.0f, -dim.chassis_width/2 + (dim.wheel_max_width));
		draw_wheel(quadric, dim, current);
		//draw_rendered_wheel();
	glPopMatrix();
	glPushMatrix();
		current.set(-dim.chassis_length/2 + dim.chassis_length/4, 0.0f, dim.chassis_width/2 - (dim.wheel_max_width));
		draw_wheel(quadric, dim, current);
		//draw_rendered_wheel();
	glPopMatrix();
	glPushMatrix();
		current.set(dim.chassis_length/2 - dim.chassis_length/4, 0.0f, -dim.chassis_width/2 + (dim.wheel_max_width));
		draw_wheel(quadric, dim, current);
		//draw_rendered_wheel();
	glPopMatrix();
	glPushMatrix();
		current.set(dim.chassis_length/2 - dim.chassis_length/4, 0.0f, +dim.chassis_width/2 - (dim.wheel_max_width));
		draw_wheel(quadric, dim, current);
		//draw_rendered_wheel();
	glPopMatrix();
}

void RobotModel::draw_drive(void)
{
	const GLfloat t=sqrtf(((dim.chassis_length/4*tan(30*pbo))*(dim.chassis_length/4*tan(30*pbo)))+((dim.wheel_o_rad+1)*(dim.wheel_o_rad+1)));
	const GLfloat tx = t*cos(60*pbo)*cos(30*pbo);
	const GLfloat ty = t*sin(60*pbo);
	const GLfloat tz = t*cos(60*pbo)*sin(30*pbo);
	
	static int i = 0;
	
	Point WG1(-dim.chassis_length/2 +dim.chassis_length/4, 0.0f, dim.chassis_width/2 + 0.74 - dim.gear.width/2);
	Point WG2(-dim.chassis_length/2 +dim.chassis_length/4, 0.0f, -dim.chassis_width/2 - 0.74 + dim.gear.width/2);
	Point WG3(dim.chassis_length/2 -dim.chassis_length/4, 0.0f, dim.chassis_width/2 + 0.76 + dim.gear.width/2);
	Point WG4(dim.chassis_length/2 -dim.chassis_length/4, 0.0f, -dim.chassis_width/2 - 0.76 - dim.gear.width/2);
	Point MG1(0.0f, ty/2, dim.chassis_width/2 + 0.74 - dim.gear.width/2);
	Point MG2(0.0f, ty/2, -dim.chassis_width/2 - 0.74 + dim.gear.width/2);
	Point MG3(0.0f, ty/2, dim.chassis_width/2 + 0.76 + dim.gear.width/2);
	Point MG4(0.0f, ty/2, -dim.chassis_width/2 - 0.76 - dim.gear.width/2);

	
	glPushMatrix();
		glTranslatef(-dim.chassis_length/2 +dim.chassis_length/4, 0.0f, dim.chassis_width/2 + 0.74 - dim.gear.width/2);
		draw_gear(quadric, dim.gear, i);
		//draw_rendered_gear();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-dim.chassis_length/2 +dim.chassis_length/4, 0.0f, -dim.chassis_width/2 - 0.74 + dim.gear.width/2);
		draw_gear(quadric, dim.gear, i);
		//draw_rendered_gear();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(dim.chassis_length/2 -dim.chassis_length/4, 0.0f, dim.chassis_width/2 + 0.76 + dim.gear.width/2);
		draw_gear(quadric, dim.gear, i);
		//draw_rendered_gear();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(dim.chassis_length/2 -dim.chassis_length/4, 0.0f, -dim.chassis_width/2 - 0.76 - dim.gear.width/2);
		draw_gear(quadric, dim.gear, i);
		//draw_rendered_gear();
	glPopMatrix();
	
	//draw motor gears
	glPushMatrix();
		glTranslatef(0.0f, ty/2, dim.chassis_width/2 + 0.76 + dim.gear.width/2);
		draw_gear(quadric, dim.gear, i);
		//draw_rendered_gear();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0f, ty/2, -dim.chassis_width/2 - 0.76 - dim.gear.width/2);
		draw_gear(quadric, dim.gear, i);
		//draw_rendered_gear();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, ty/2, dim.chassis_width/2 + 0.74 - dim.gear.width/2);
		draw_gear(quadric, dim.gear, i);
		//draw_rendered_gear();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0f, ty/2, -dim.chassis_width/2 - 0.74 + dim.gear.width/2);
		draw_gear(quadric, dim.gear, i);
		//draw_rendered_gear();
	glPopMatrix();
	i++;
	
	//miscellaneous motor parts
	glEnable(GL_CULL_FACE);
	glPushMatrix();
	{
		glColor3f(0.3f,0.2f,0.2f);
		glTranslatef(0.0f, ty/2, dim.chassis_width/2-0.25f-0.25f);
		gluCylinder(quadric, dim.gear.i_rad, dim.gear.o_rad, 0.25f, 32, 2);
		glFrontFace(GL_CW);
		gluDisk(quadric, 0.0f, dim.gear.i_rad, 32, 1);
		glFrontFace(GL_CCW);
		glTranslatef(0.0f, 0.0f, 0.25f);
		gluCylinder(quadric, dim.gear.o_rad, dim.gear.o_rad, 0.1f, 32, 2);
		glTranslatef(0.0f, 0.0f, 0.1f);
		gluCylinder(quadric, dim.gear.o_rad, dim.gear.i_rad, 0.25f, 32, 2);
		glTranslatef(0.0f, 0.0f, 0.25f);
		gluCylinder(quadric, dim.gear.i_rad, dim.gear.i_rad, 2.0f, 32, 2);
		glTranslatef(0.0f, 0.0f, 2.0f);
		gluDisk(quadric, 0.0f, dim.gear.i_rad+0.1f, 32, 2);
	}
	glPopMatrix();
	
	glPushMatrix();
	{
		glColor3f(0.3f,0.2f,0.2f);
		glTranslatef(0.0f, ty/2, -dim.chassis_width/2 +0.25f-0.4f);
		gluCylinder(quadric, dim.gear.i_rad, dim.gear.o_rad, 0.25f, 32, 2);
		glTranslatef(0.0f, 0.0f, 0.25f);
		gluCylinder(quadric, dim.gear.o_rad, dim.gear.o_rad, 0.1f, 32, 2);
		glTranslatef(0.0f, 0.0f, 0.1f);
		gluCylinder(quadric, dim.gear.o_rad, dim.gear.i_rad, 0.25f, 32, 2);	
		glTranslatef(0.0f, 0.0f, 0.25f);
		gluDisk(quadric, 0.0f, dim.gear.i_rad, 32, 1);
		glColor3f(0.3f,0.2f,0.2f);
		glTranslatef(0.0f, 0.0f, -2.60f);
		gluCylinder(quadric, dim.gear.i_rad, dim.gear.i_rad, 2.0f, 32, 2);
		glFrontFace(GL_CW);
		gluDisk(quadric, 0.0f, dim.gear.i_rad+0.1f, 32, 2);
	}
	glPopMatrix();
	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);
	
	//draw motor
	glPushMatrix();
	{
		glColor3f(0.1f, 0.1f, 0.1f);
		glTranslatef(dim.gear.o_rad, (ty/2), dim.chassis_width/2 -0.25f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		gluCylinder(quadric, 0.8f, 0.8f, 2.0f, 20, 2);
		glTranslatef(0.0f, 0.0f, -0.2f);
		gluCylinder(quadric, 0.0f, 0.8f, 0.2f, 32, 2);
		glTranslatef(0.0f, 0.0f, 2.2f);
		gluDisk(quadric, 0.5f, 0.8f, 20, 2);
		gluCylinder(quadric, 0.5f, 0.4f, 0.2f, 20, 2);
		glTranslatef(0.0f, 0.0f, 0.2f);
		glColor3f(0.1f, 0.1f, 0.1f);
		gluDisk(quadric, 0.0f, 0.4f, 20, 2);
	}
	glPopMatrix();
	//supports
	glPushMatrix();
	{
		glColor3f(0.4f, 0.4, 0.5f);	
		glTranslatef(dim.gear.o_rad+0.5, 0.0f, dim.chassis_width/2 -0.25f);
		glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
		gluCylinder(quadric, 0.22f, 0.2f, ty/2, 20, 2);
		glTranslatef(1.0f, 0.0f, 0.0f);
		gluCylinder(quadric, 0.22f, 0.2f, ty/2, 20, 2);
	}
	glPopMatrix();
	
	glPushMatrix();
	{
		glColor3f(0.1f, 0.1f, 0.1f);
		glTranslatef(dim.gear.o_rad, (ty/2), -dim.chassis_width/2 +0.25f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		gluCylinder(quadric, 0.8f, 0.8f, 2.0f, 20, 2);
		glTranslatef(0.0f, 0.0f, -0.2f);
		gluCylinder(quadric, 0.0f, 0.8f, 0.2f, 32, 2);
		glTranslatef(0.0f, 0.0f, 2.2f);
		gluDisk(quadric, 0.5f, 0.8f, 20, 2);
		gluCylinder(quadric, 0.5f, 0.4f, 0.2f, 20, 2);
		glTranslatef(0.0f, 0.0f, 0.2f);
		glColor3f(0.1f, 0.1f, 0.1f);
		gluDisk(quadric, 0.0f, 0.4f, 20, 2);
	}
	glPopMatrix();
	//supports
	glPushMatrix();
	{
		glColor3f(0.4f, 0.4, 0.5f);	
		glTranslatef(dim.gear.o_rad+0.5, 0.0f, -dim.chassis_width/2 +0.25f);
		glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
		gluCylinder(quadric, 0.22f, 0.2f, ty/2, 20, 2);
		glTranslatef(1.0f, 0.0f, 0.0f);
		gluCylinder(quadric, 0.22f, 0.2f, ty/2, 20, 2);
	}
	glPopMatrix();
	
	glDisable(GL_CULL_FACE);
	
	//chains
	glPushMatrix();
	{
		//close, front
		glPushMatrix();
		{
			Point zero;
			Point c1(MG1.x()-WG1.x(), MG1.y()-WG1.y(), MG1.z()-WG1.z());
			glTranslatef(WG1.x(), WG1.y(), WG1.z());
			glRotatef(atan((MG1.y()-WG1.y())/(MG1.x()-WG1.x()))/pbo, 0.0f, 0.0f, 1.0f);
			draw_chain(quadric, dim.gear, zero, c1, 8);
		}
		glPopMatrix();
		//far, front
		glPushMatrix();
		{
			Point zero;
			Point c2(MG2.x()-WG2.x(), MG2.y()-WG2.y(), MG2.z()-WG2.z());
			glTranslatef(WG2.x(), WG2.y(), WG2.z());
			glRotatef(atan((MG2.y()-WG2.y())/(MG2.x()-WG2.x()))/pbo, 0.0f, 0.0f, 1.0f);
			draw_chain(quadric, dim.gear, zero, c2, 8);
		}
		glPopMatrix();
		//close, rear
		glPushMatrix();
		{
			Point zero;
			Point c3(-MG3.x()+WG3.x(), -MG3.y()+WG3.y(), -MG3.z()+WG3.z());
			glTranslatef(MG3.x(), MG3.y(), MG3.z());
			glRotatef(atan((MG3.y()-WG3.y())/(MG3.x()-WG3.x()))/pbo, 0.0f, 0.0f, 1.0f);
			draw_chain(quadric, dim.gear, zero, c3, 8);
		}
		glPopMatrix();
		//far, rear
		glPushMatrix();
		{
			Point zero;
			Point c4(-MG4.x()+WG4.x(), -MG4.y()+WG4.y(), -MG4.z()+WG4.z());
			glTranslatef(MG4.x(), MG4.y(), MG4.z());
			glRotatef(atan((MG4.y()-WG4.y())/(MG4.x()-WG4.x()))/pbo, 0.0f, 0.0f, 1.0f);
			draw_chain(quadric, dim.gear, zero, c4, 8);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void RobotModel::draw_body(void)
{
	//const GLfloat t=5*tan(30*pbo);
	const GLfloat t=sqrtf(((dim.chassis_length/4*tan(30*pbo))*(dim.chassis_length/4*tan(30*pbo)))+((dim.wheel_o_rad+1)*(dim.wheel_o_rad+1)));
	const GLfloat tx = t*cos(60*pbo)*cos(30*pbo);
	const GLfloat ty = t*sin(60*pbo);
	const GLfloat tz = t*cos(60*pbo)*sin(30*pbo);
	
	glEnable(GL_CULL_FACE);
	
	glColor3f(0.7f,0.7f,0.7f);
	glPushMatrix();
		glTranslatef((-dim.chassis_length/2)+0.5, 0.0f, (dim.chassis_width/2)-0.5);
		glRotatef(120.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
		gluCylinder(quadric, 0.2f, 0.2f, t, 32, 2);
		//gluCylinder(quadric, 0.2f, 0.2f, sqrt( 25 + pow(t,2) +(4.5*4.5)), 32, 2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef((dim.chassis_length/2)-0.5, 0.0f, (dim.chassis_width/2)-0.5);
		glRotatef(120.0f, 0.0f, -1.0f, 0.0f);
		glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
		gluCylinder(quadric, 0.2f, 0.2f, t, 32, 2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef((-dim.chassis_length/2)+0.5, 0.0f, (-dim.chassis_width/2)+0.5);
		glRotatef(60.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
		gluCylinder(quadric, 0.2f, 0.2f, t, 32, 2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef((dim.chassis_length/2)-0.5, 0.0f, (-dim.chassis_width/2)+0.5);
		glRotatef(60.0f, 0.0f, -1.0f, 0.0f);
		glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
		gluCylinder(quadric, 0.2f, 0.2f, t, 32, 2);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef((-dim.chassis_length/2)+tx+0.4, ty, -dim.chassis_width/2+tz+0.2);
		gluCylinder(quadric, 0.2f, 0.2f, dim.chassis_width-(2*tz)-0.4, 32, 2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef((dim.chassis_length/2)-tx-0.4, ty, -dim.chassis_width/2+tz+0.2);
		gluCylinder(quadric, 0.2f, 0.2f, dim.chassis_width-(2*tz)-0.4, 32, 2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef((dim.chassis_length/2)-tx-0.4, ty, -dim.chassis_width/2+tz+0.4);
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		gluCylinder(quadric, 0.2f, 0.2f, dim.chassis_length-(ty)-0.8, 32, 2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef((dim.chassis_length/2)-tx-0.4, ty, dim.chassis_width/2-tz-0.4);
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		gluCylinder(quadric, 0.2f, 0.2f, dim.chassis_length-(ty)-0.8, 32, 2);
	glPopMatrix();
	
	glDisable(GL_CULL_FACE);
}
