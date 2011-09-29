#ifndef MOUSEDRIVE_DRAWINGAREA
#define MOUSEDRIVE_DRAWINGAREA

#include <gtkmm.h>
#include <gtkglmm.h>
//#include "gui_shared_data.h"
#include "Singleton.hh"
#include <libglademm/xml.h>
#include "bitmap.h"
#include <string>
#include <GL/gl.h>
#include "CIRC.hh"

class driveview : public Gtk::DrawingArea, public Gtk::GL::Widget, public Comrade::Corelib::Singleton<driveview>
{
	public:
		driveview();
		~driveview();
		void draw(void);
		void update(Comrade::Corelib::speed_update& su);
	protected:
		virtual void on_realize();
		virtual bool on_configure_event(GdkEventConfigure* event);
		virtual bool on_expose_event(GdkEventExpose* event);
		virtual bool on_button_press_event(GdkEventButton* event);
		virtual bool on_button_release_event(GdkEventButton* event);
		virtual bool on_motion_notify_event(GdkEventMotion* event);	
	private:
		void set_pos(GLfloat x, GLfloat y);
		GLfloat x_pos, y_pos;
		void drawdrive();
		int buttonpressed;
};

#endif
