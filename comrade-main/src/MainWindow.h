#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <sigc++/sigc++.h>
#include <libglademm/xml.h>
#include <gtkmm.h>

#include "GridDrawingArea.h"

namespace Comrade
{
	
namespace LocalGUI
{

class MainWindow : public Gtk::Window
{
	public:
		MainWindow(Glib::RefPtr<Gnome::Glade::Xml>& refGlade);
		virtual ~MainWindow();
	protected:
		Glib::RefPtr<Gnome::Glade::Xml> m_refGlade;
		GridDrawingArea *gda;
		Gtk::MenuItem *menu_quit;
		Gtk::VScale *zoom_slider;
	
	//signal handlers
		void on_menu_quit();
		void on_zoom_changed();
		//void on_autonomous_clicked();
};

	
}
}

#endif
