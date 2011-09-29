#include <iostream>
#include <cstdlib>
#include <gtkmm.h>
#include <gtkglmm.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "CameraDA.hh"
#include "bitmap.h"
#include <string>
#include "CIRC.hh"
#include "MouseDriveDA.hh"
#include <sigc++/sigc++.h>

using namespace Comrade::Corelib;

driveview::driveview() : Gtk::GL::Widget(static_cast<Gtk::Widget&>(*this))
{
	//set_pos(0.0f, 0.0f);
	x_pos = y_pos = 0;
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
	CIRC::get_singleton().on_speed_update_receive.connect(sigc::mem_fun(this, &driveview::update));
}

driveview::~driveview()
{
}

void driveview::drawdrive(void)
{
	glPolygonMode(GL_FRONT, GL_LINE);
	glLineWidth(2.5f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x_pos, y_pos);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(1.0f, -1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);
		glVertex2f(-1.0f, -1.0f);
	glEnd();
}

void driveview::set_pos(GLfloat x, GLfloat y)
{
	/*
	x_pos=(x-128)*(get_width()/2)/255;
	y_pos=(y-128)*(get_height()/2)/255;
	if(x_pos<-1)
		x_pos=-1;
	else if(x_pos>1)
		x_pos=1;
	if(y_pos<-1)
		y_pos=-1;
	else if(y_pos>1)
		y_pos=1;
	*/
	x_pos=(x-128)/128;
	y_pos=(y-128)/128;
}

void driveview::draw(void)
{

    glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
		drawdrive();
	glPopMatrix();
}

void driveview::update(speed_update& su)
{
	Glib::RefPtr<Gdk::Window> window = get_window();
	window->invalidate_rect(Gdk::Rectangle(0,0,get_width(),get_height()),false);
	set_pos(su.left, su.right);
}

void driveview::on_realize()
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
	/*
	if(get_width()>get_height())
		glViewport(0,0,h,h);
	else
		glViewport(0,0,w,w);

	glOrtho(-1,1,-1,1,1,-1);
	//glOrtho(-h/2,h/2,-w/2,w/2,-1,1);
	*/
	glViewport(0,0,get_width(),get_height());
        glOrtho(-40,40,-40,40,0,0);

	
	glwindow->gl_end();
	// *** OpenGL END ***
	add_events(Gdk::ALL_EVENTS_MASK);

	std::cerr << "\nfinished gl onrealize";
}

bool driveview::on_configure_event(GdkEventConfigure* event)
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
	/*
	if(w>h)
		glViewport(0,0,h,h);
	else
		glViewport(0,0,w,w);
	//glOrtho(-h/2,h/2,-w/2,w/2,-1,1);
	glOrtho(-1,1,-1,1,1,-1);
	*/
	
	glViewport(0,0,get_width(),get_height());
        glOrtho(-40,40,-40,40,0,0);
	
	glwindow->gl_end();
	// *** OpenGL END ***
	//std::cerr<<"\nfinished onconfigure";
	return true;
}

bool driveview::on_expose_event(GdkEventExpose* event)
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

bool driveview::on_button_press_event(GdkEventButton *event)
{
	//cout << "Button press!\n";
	//cout << " Button: " << event->button << "\n x     : " << event->x << "\n y     : "<<event->y << "\n\n";
	//double speed=(oldheight/2)-event->y;
	if (event->button==1)
	{
		buttonpressed=event->button;
	}
	else
	{
		buttonpressed=false;
	}
	return true;
}

bool driveview::on_button_release_event(GdkEventButton *event)
{
	//cout << "Button release!\n";
	buttonpressed=0;
	//stop();
	speed_update su;
	su.left=128;
	su.right=128;
	CIRC::get_singleton().send_set_speeds(su);
	return true;
}

bool driveview::on_motion_notify_event (GdkEventMotion *event)
{
	//cout << "Motion notify!\n";
	if (buttonpressed && event && event->window)
	{
		const Glib::RefPtr<Gdk::Window> refWindow =
			Glib::wrap((GdkWindowObject*) event->window,true);
		if (refWindow)
		{
			int x=0,y=0;
			Gdk::ModifierType state = Gdk::ModifierType(0);
			refWindow->get_pointer(x,y,state);
			//cout << buttonstartx-x << " " << buttonstarty-y << endl; 
			if (buttonpressed==1)
			{
				/* for 1st person view */
				speed_update su;
				su.left=x*256/get_width();
				su.right=y*256/get_height();
				CIRC::get_singleton().send_set_speeds(su);
			}
		}
	}
	return true;
}
