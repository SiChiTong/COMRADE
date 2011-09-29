#include "MainWindow.h"
#include "ExecutionEngine.h"
#include <iostream>

using namespace Comrade::LocalGUI;
using namespace Comrade::ComradeRuntime;
using namespace std;

MainWindow::MainWindow(Glib::RefPtr<Gnome::Glade::Xml>& refGlade):
	m_refGlade(refGlade)
{
	set_title("Comrade");
	set_default_size(800,600);
	m_refGlade->reparent_widget("vbox1",*this);
	m_refGlade->get_widget_derived("GridDrawingArea",gda);
	//m_refGlade->get_widget_derived("MouseControlDrawingArea",mousedrive);	
	m_refGlade->get_widget("menu_quit",menu_quit);
	m_refGlade->get_widget("ZoomSlider",zoom_slider);
	//m_refGlade->get_widget("autonomous_checkbutton",autonomous_checkbutton);
	menu_quit->signal_activate().connect(sigc::mem_fun(this,&MainWindow::on_menu_quit));
	zoom_slider->signal_value_changed().connect(sigc::mem_fun(this,&MainWindow::on_zoom_changed));
	//autonomous_checkbutton->signal_clicked().connect(sigc::mem_fun(this,&MainWindow::on_autonomous_clicked));
	show_all_children();
	show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_menu_quit()
{
	hide();
}

void MainWindow::on_zoom_changed()
{
	gda->set_zoom(int(zoom_slider->get_value()));
}

/*void MainWindow::on_autonomous_clicked()
{
	cout << "foo" << endl;
	bool state=autonomous_checkbutton->get_active();
	ExecutionEngine *ee;
	if (ee=ExecutionEngine::get_singleton_ptr())
	{
		if (state) ee->get_singleton().connect_sonar();
		if (!state) ee->get_singleton().disconnect_sonar();
	}
}*/
