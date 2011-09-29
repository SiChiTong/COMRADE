#include <iostream>
#include <cstdlib>
#include <glibmm/main.h>
#include <gtkmm.h>
#include <gtkglmm.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "CameraDA.hh"
#include "bitmap.h"
#include <string.h>

cameraview::cameraview() : Gtk::GL::Widget(static_cast<Gtk::Widget&>(*this))
{
	BitmapBits = 0;
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
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &cameraview::update), 200);
}

cameraview::~cameraview()
{
	free((void*)BitmapBits);
}

void cameraview::setimage(char* filename)
{
	if(BitmapBits)
		free((void*)BitmapBits);
	imagefile = filename;
	BitmapBits = LoadDIBitmap(filename, &BitmapInfo);
}

void cameraview::drawimage(void)
{
	/*glDrawPixels(BitmapInfo->bmiHeader.biWidth,
				 BitmapInfo->bmiHeader.biHeight,
				 GL_BGR_EXT, GL_UNSIGNED_BYTE, BitmapBits);*/
	//glRasterPos2i(0,0);
	//glPixelZoom(w()/320, h()/240);
	glDrawPixels(320, 240,GL_RGB, GL_UNSIGNED_BYTE, BitmapBits);
	//glPixelZoom(w()/320, h()/240);
	//update();
}

void cameraview::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);//w()/2, h()/2, 0.0f);
		drawimage();
	glPopMatrix();	
}



bool cameraview::update()
{
	Glib::RefPtr<Gdk::Window> window = get_window();
	window->invalidate_rect(Gdk::Rectangle(0,0,get_width(),get_height()),false);
	if(BitmapBits)
		free((void*)BitmapBits);
	BitmapBits = LoadDIBitmap(imagefile.c_str(), &BitmapInfo);
	return true;
}

void cameraview::on_realize()
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
	glViewport(0,0,w,h);
        glOrtho(0,320,0,240,0,0);
	
	glwindow->gl_end();
	// *** OpenGL END ***
	add_events(Gdk::ALL_EVENTS_MASK);

	std::cerr << "\nfinished gl onrealize";
}

bool cameraview::on_configure_event(GdkEventConfigure* event)
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
	
	glViewport(0,0,w,h);
        glOrtho(0,320,0,240,0,0);
	
	glwindow->gl_end();
	// *** OpenGL END ***
	//std::cerr<<"\nfinished onconfigure";
	return true;
}

bool cameraview::on_expose_event(GdkEventExpose* event)
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
