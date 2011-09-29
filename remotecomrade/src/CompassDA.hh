#ifndef COMPASS_DRAWINGAREA
#define COMPASS_DRAWINGAREA

#include <gtkmm.h>
#include <gtkglmm.h>
//#include "gui_shared_data.h"
#include "Singleton.hh"
#include <libglademm/xml.h>


class compassview : public Gtk::DrawingArea, public Gtk::GL::Widget, public Comrade::Corelib::Singleton<compassview>
{
	public:
		compassview();
		~compassview();
		void set_angle(GLfloat new_angle);
		GLfloat get_angle(void);
		void update(double robot_orientation);
		void draw(void);
	protected:
		virtual void on_realize();
		virtual bool on_configure_event(GdkEventConfigure* event);
		virtual bool on_expose_event(GdkEventExpose* event);
	private:
		GLUquadricObj *quadric;
		GLfloat angle;
		void drawcompass(void);
};

#endif
