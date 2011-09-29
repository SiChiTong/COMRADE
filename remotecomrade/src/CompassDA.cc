#include <iostream>
#include <cstdlib>
#include <gtkmm.h>
#include <gtkglmm.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "CompassDA.hh"

compassview::compassview() : Gtk::GL::Widget(static_cast<Gtk::Widget&>(*this))
{
	// OpenGL rendering context
	Glib::RefPtr<Gdk::GL::Config> glconfig;

	// Try double-buffered visual
	glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH  | Gdk::GL::MODE_DOUBLE | Gdk::GL::MODE_STENCIL);
	
	if(!glconfig)
	{
		std::cerr << "\ncould not enable double buffering";
		glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH | Gdk::GL::MODE_STENCIL);
		if(!glconfig)
		{
			std::cerr << "\ncould not find any GL visual. don't know what to do.";
		}
	}
	
	//GLConfigUtil::examine_gl_attrib(glconfig);
	
	set_gl_capability(glconfig);
	
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	
	
	quadric = gluNewQuadric();
	angle = 0.0f;
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
}

compassview::~compassview()
{
	gluDeleteQuadric(quadric);
}

void compassview::drawcompass(void)
{
//	int w=get_width();
//	int h=get_height();
	

	glPushMatrix();
		glRotatef(angle, 0.0f, 0.0f, 1.0f);
		glPushMatrix();
			glColor3f(0.0f, 0.8f, 0.8f);
			//glTranslatef(0.0f, 0.75f, 0.0f);
			glBegin(GL_LINE_STRIP);//N
				glVertex3f(-0.05f, 0.75f, 0.0f);
				glVertex3f(-0.05f, 0.88f, 0.0f);
				glVertex3f(0.05f, 0.75f, 0.0f);
				glVertex3f(0.05f, 0.88f, 0.0f);
			glEnd();
			glBegin(GL_LINE_STRIP);//S
				glVertex3f(0.05f, -0.75f, 0.0f);
				glVertex3f(-0.03f, -0.75f, 0.0f);
				glVertex3f(-0.05f, -0.78f, 0.0f);
				glVertex3f(-0.03f, -0.81f, 0.0f);
				glVertex3f(0.03f, -0.81f, 0.0f);
				glVertex3f(0.05f, -0.84f, 0.0f);
				glVertex3f(0.03f, -0.88f, 0.0f);
				glVertex3f(-0.05f, -0.88f, 0.0f);
			glEnd();
			glBegin(GL_LINE_STRIP);//E
				glVertex3f(0.88f, 0.05f, 0.0f);
				glVertex3f(0.78f, 0.05f, 0.0f);
				glVertex3f(0.78f, -0.05f, 0.0f);
				glVertex3f(0.88f, -0.05f, 0.0f);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(0.78f, 0.0f, 0.0f);
				glVertex3f(0.85f, 0.0f, 0.0f);
			glEnd();
			glBegin(GL_LINE_STRIP);//W
				glVertex3f(-0.88f, 0.05f, 0.0f);
				glVertex3f(-0.85f, -0.05f, 0.0f);
				glVertex3f(-0.82f, 0.01f, 0.0f);
				glVertex3f(-0.79f, -0.05f, 0.0f);
				glVertex3f(-0.76f, 0.05f, 0.0f);
			glEnd();
		glPopMatrix();	
	glPopMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
		glVertex3f(0.0, 0.70, 0.0);
		glVertex3f(0.20, 0.0, 0.0);
		glVertex3f(0.0, -0.70, 0.0);
		glVertex3f(-0.20, 0.0, 0.0);
	glEnd();

	glColor3f(0.0f, 1.0f, 1.0f);
	gluDisk(quadric, 0.90, 0.95, 32, 32);
	glColor3f(0.2f, 0.2f, 0.2f);
	gluDisk(quadric, 0.0, 0.10, 16,16);
}

void compassview::set_angle(GLfloat new_angle)
{
	angle = new_angle;
}

GLfloat compassview::get_angle(void)
{
	return(angle);
}

void compassview::draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
		//glTranslatef(0.0f, 0.0f, 0.0f);//w()/2, h()/2, 0.0f);
		drawcompass();
	glPopMatrix();
}

void compassview::update(double robot_orientation)
{
	angle = robot_orientation;
	Glib::RefPtr<Gdk::Window> window = get_window();
	window->invalidate_rect(Gdk::Rectangle(0,0,get_width(),get_height()),false);
}

void compassview::on_realize()
{
	// We need to call the base on_realize()
	Gtk::DrawingArea::on_realize();
	
	//
	// Get GL::Window.
	//
	
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	
	//
	// GL calls.
	//
	
	// *** OpenGL BEGIN ***
	if (!glwindow->gl_begin(get_gl_context()))
		return;
	
	glLoadIdentity();
	int w = get_width(), h = get_height();
	if(get_width()>get_height())
		glViewport(0,0,h,h);
	else
		glViewport(0,0,w,w);

	glOrtho(-1,1,-1,1,1,-1);
	//glOrtho(-h/2,h/2,-w/2,w/2,-1,1);
	
	glwindow->gl_end();
	// *** OpenGL END ***
	add_events(Gdk::ALL_EVENTS_MASK);

	std::cerr << "\nfinished gl onrealize";
}

bool compassview::on_configure_event(GdkEventConfigure* event)
{
	//
	// Get GL::Window.
	//
	//std::cerr<<"\nonconfigure";
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	
	//
	// GL calls.
	//
	
	// *** OpenGL BEGIN ***
	if (!glwindow->gl_begin(get_gl_context()))
	return false;
	int h=get_height();
	int w=get_width();
	if(w>h)
		glViewport(0,0,h,h);
	else
		glViewport(0,0,w,w);
	//glOrtho(-h/2,h/2,-w/2,w/2,-1,1);
	glOrtho(-1,1,-1,1,1,-1);
	glwindow->gl_end();
	// *** OpenGL END ***
	//std::cerr<<"\nfinished onconfigure";
	return true;
}

bool compassview::on_expose_event(GdkEventExpose* event)
{
	//
	// Get GL::Window.
	//
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	
	//
	// GL calls.
	//
	
	// *** OpenGL BEGIN ***
	if (!glwindow->gl_begin(get_gl_context()))
		return false;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	// put drawing stuff here
	draw();
	  
	// Swap buffers.
	if (glwindow->is_double_buffered())
  		glwindow->swap_buffers();
	else
  		glFlush();

	glwindow->gl_end();
	// *** OpenGL END ***
	
	return true;
}
