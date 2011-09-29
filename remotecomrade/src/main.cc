/* Always include glibmm/gtkmm header files before all the IRIS includes.
   We spent a night debugging it, without any result! */

#include "MainWindow.hh"
#include "CorelibObject.hh"
#include "CIRC.hh"
#include "SectorDraw.hh"
#include "EvidenceGrid.hh"
#include "MapUpdater.hh"
#include <sigc++/sigc++.h>
#include <iostream>

using namespace std;
using namespace Comrade::Corelib;

int main(int argc, char **argv)
{
	Gtk::Main kit(argc,argv);
	Gtk::GL::init(argc, argv);
	Glib::RefPtr<Gnome::Glade::Xml> m_refGlade;

	Comrade::Corelib::CorelibObject corelibobject;
	CIRC circ;

	Comrade::GUI::MainWindow *mainwindow;
	m_refGlade=Gnome::Glade::Xml::create("../ui/mainwindow.glade");
	mainwindow=new Comrade::GUI::MainWindow(m_refGlade);
	
	MapUpdater m;
	circ.on_map_update_receive.connect(sigc::mem_fun(m, &MapUpdater::updater));
	
	kit.run(*mainwindow);
}
