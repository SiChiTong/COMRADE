#include "LocalGUI.h"

using namespace Comrade::LocalGUI;

LocalGUI::LocalGUI()
{
	m_refGlade=Gnome::Glade::Xml::create("../ui/mainwindow.glade");
	mainwindow=new MainWindow(m_refGlade);
}

LocalGUI::~LocalGUI()
{
}

Gtk::Window& LocalGUI::get_mainwindow()
{
	return *mainwindow;
}
