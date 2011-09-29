#ifndef GRIDDRAWINGAREA_H
#define GRIDDRAWINGAREA_H

#include <gtkmm/drawingarea.h>
#include <gdkmm/color.h>
#include <gdkmm/types.h>
#include <glibmm/arrayhandle.h>
#include <libglademm.h>

#include "EvidenceGrid.hh"
#include "Singleton.hh"

namespace Comrade
{
namespace LocalGUI
{

class GridDrawingArea: public Gtk::DrawingArea, public Comrade::Corelib::Singleton<GridDrawingArea>
{	
	public:
		int destdir;
		GridDrawingArea(BaseObjectType *cobject, const Glib::RefPtr<Gnome::Glade::Xml>& refGlade);
		~GridDrawingArea();
		bool on_expose_event(GdkEventExpose *event);
//		bool on_motion_notify_event(GdkEventMotion *event);
		void on_realize();
		void update();
		//void updatePoint(int cx, int cy);
		void set_evidence_grid(Comrade::Corelib::EvidenceGrid<unsigned char> *grid);
		void set_zoom(int zoom);
		void update_point(int x, int y, unsigned char color);
		void set_center(int x, int y);
		void refresh_pixmap();
	
	private:
		Glib::RefPtr<Gdk::GC> gc_;
		Glib::RefPtr<Gdk::Pixmap> pixmap;
		Glib::RefPtr<Gdk::Window> window;
		Gdk::Color backgroundColor;
		Gdk::Color gridColor;
		Gdk::Color scanColor;
		Gdk::Color centerColor;
		Gdk::Color colorgradient[256];
		int width,height;
		int centerx,centery;
		int egstartx, egstarty;
		int egendx, egendy;
		int zoom;
		bool dirty;
	
		Glib::RefPtr<Gnome::Glade::Xml> m_refGlade;
		Comrade::Corelib::EvidenceGrid<unsigned char> *eg;
};

}
}

#endif
