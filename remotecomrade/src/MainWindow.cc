#include "MainWindow.hh"
#include <iostream>
#include "CIRC.hh"

using namespace Comrade::GUI;
using namespace Comrade::Corelib;
using namespace std;

MainWindow::MainWindow(Glib::RefPtr<Gnome::Glade::Xml>& refGlade):
	m_refGlade(refGlade), Gtk::GL::Widget(static_cast<Gtk::Widget&>(*this))
{
	current_mode = 3;
	set_title("Comrade");
	set_default_size(800,600);
	set_reallocate_redraws(true);
	m_refGlade->reparent_widget("vbox1",*this);
	cout << "grid drawing area" << endl;
	m_refGlade->get_widget_derived("GridDrawingArea",gda);
	cout << "gl drawing area" << endl;
	gla=new GL3DScene;
	cda=new compassview;
	camerada=new cameraview;
	camerada->setimage("adam.bmp");
	mousedrive = new driveview;
	m_refGlade->get_widget("alignment2",alignment2);
	alignment2->add(*gla);
	m_refGlade->get_widget("alignment5", alignment5);
	alignment5->add(*cda);
	m_refGlade->get_widget("alignment7", alignment7);
	alignment7->add(*camerada);
	m_refGlade->get_widget("alignment6", alignment6);
	alignment6->add(*mousedrive);
	m_refGlade->get_widget("start_button", start_button);
	start_button->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_start_button_clicked));
	m_refGlade->get_widget("stop_button", stop_button);
	stop_button->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_stop_button_clicked));
	m_refGlade->get_widget("mode_lf", mode_lf);
	mode_lf->signal_toggled().connect(sigc::mem_fun(this, &MainWindow::on_mode_toggle));
	m_refGlade->get_widget("mode_oo", mode_oo);
	mode_oo->signal_toggled().connect(sigc::mem_fun(this, &MainWindow::on_mode_toggle));
	m_refGlade->get_widget("mode_to", mode_to);
	mode_to->signal_toggled().connect(sigc::mem_fun(this, &MainWindow::on_mode_toggle));
//	m_refGlade->get_widget_derived("GLDrawingArea", gla);
//	m_refGlade->get_widget("menu_quit",menu_quit);
//	m_refGlade->get_widget("ZoomSlider",zoom_slider);
//	menu_quit->signal_activate().connect(sigc::mem_fun(this,&MainWindow::on_menu_quit));
//	zoom_slider->signal_value_changed().connect(sigc::mem_fun(this,&MainWindow::on_zoom_changed));
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

//void MainWindow::on_zoom_changed()
//{
//	gda->set_zoom(int(zoom_slider->get_value()));
//}

void MainWindow::on_start_button_clicked()
{
	CIRC::get_singleton().send_start(current_mode);
	std::cerr << "\nsending " << (int)current_mode;
}

void MainWindow::on_stop_button_clicked()
{
	CIRC::get_singleton().send_start(0);
}

void MainWindow::on_mode_toggle(void)
{
	if(mode_lf->get_active())
	{
		current_mode=3;
	}
	else if(mode_oo->get_active())
	{
		current_mode=2;
	}
	else
	{
		current_mode=1;
	}
}
