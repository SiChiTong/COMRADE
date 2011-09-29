#ifndef LOCALGUI_H
#define LOCALGUI_H

#include <sigc++/sigc++.h>
#include <libglademm/xml.h>
#include <gtkmm.h>

#include "MainWindow.h"

namespace Comrade
{
	
namespace LocalGUI
{

class LocalGUI
{
	public:
		LocalGUI();
		virtual ~LocalGUI();
		Gtk::Window& get_mainwindow();
	protected:
		Glib::RefPtr<Gnome::Glade::Xml> m_refGlade;
		MainWindow* mainwindow;
};

}
}

#endif
