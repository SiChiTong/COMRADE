#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <sigc++/sigc++.h>
#include <libglademm/xml.h>
#include <gtkmm.h>

#include "GridDrawingArea.hh"
#include "GLDrawingArea.hh"
#include "CompassDA.hh"
#include "CameraDA.hh"
#include "MouseDriveDA.hh"
#include <gtkglmm.h>

namespace Comrade
{
	
namespace GUI
{

class MainWindow : public Gtk::Window, public Gtk::GL::Widget
{
	public:
		MainWindow(Glib::RefPtr<Gnome::Glade::Xml>& refGlade);
		virtual ~MainWindow();
	protected:
		Glib::RefPtr<Gnome::Glade::Xml> m_refGlade;
		GridDrawingArea *gda;
		GL3DScene *gla;
		compassview *cda;
		cameraview *camerada;
		driveview *mousedrive;
		Gtk::MenuItem *menu_quit;
		Gtk::VScale *zoom_slider;
		Gtk::Alignment *alignment2, *alignment5, *alignment6, *alignment7;
		Gtk::Button *start_button, *stop_button;
		Gtk::RadioButton *mode_lf, *mode_oo, *mode_to;
	
		unsigned char current_mode;
	
	//signal handlers
		void on_menu_quit();
		//void on_zoom_changed();
		//void send_start();
		//void send_stop();
		void on_start_button_clicked();
		void on_stop_button_clicked();
		void on_mode_toggle();
};

	
}
}

#endif
