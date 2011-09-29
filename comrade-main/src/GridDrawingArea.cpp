#include "GridDrawingArea.h"

#include <gtkmm/style.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gdkmm/window.h>
#include <gdkmm/drawable.h>
#include <gdkmm/gc.h>
#include <gdkmm/color.h>
#include <gdkmm/colormap.h>
#include <glibmm/arrayhandle.h>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

using namespace Comrade::LocalGUI;
using namespace std;

GridDrawingArea::GridDrawingArea(BaseObjectType *cobject, const Glib::RefPtr<Gnome::Glade::Xml>& refGlade):
	Gtk::DrawingArea(cobject),
	centerx(400),
	centery(300),
	m_refGlade(refGlade),
	eg(NULL),
	dirty(false)
{
	height=width=0;
	zoom=1;
}

GridDrawingArea::~GridDrawingArea()
{
}

void GridDrawingArea::set_evidence_grid(Comrade::Corelib::EvidenceGrid<unsigned char> *grid)
{
	eg=grid;
	eg->on_update.clear();
	eg->on_update.connect(sigc::mem_fun(this,&GridDrawingArea::update_point));
}

void GridDrawingArea::update()
{
	Glib::RefPtr<Gdk::Window> window = get_window();
	window->invalidate_rect(Gdk::Rectangle(0,0,get_width(),get_height()),false);	
}

void GridDrawingArea::update_point(int x, int y, unsigned char color)
{
	if (pixmap)
	{
		int px=(x-egstartx)*zoom;
		int py=(y-egstarty)*zoom;
		gc_->set_foreground(colorgradient[color]);
		//cout << px << " " << py << endl;
		pixmap->draw_rectangle(gc_,true,px,py,zoom,zoom);
	}
}

void GridDrawingArea::refresh_pixmap()
{
	egstartx=centerx-((width/2)/zoom);
	egstarty=centery-((height/2)/zoom);
	egendx=egstartx+(width/zoom);
	egendy=egstarty+(height/zoom);
	if (egstartx<0) egstartx=0;
	if (egstarty<0) egstarty=0;
	int egx=eg->return_x();
	int egy=eg->return_y();
	if (egendx>egx) egendx=egx;
	if (egendy>egy) egendy=egy;
	//cout << egstartx << " " << egstarty << " " << egendx << " " << egendy << endl;
	for (int i=egstartx; i<egendx;i++)
		for (int j=egstarty; j<egendy; j++)
			update_point(i,j,eg->at(0,j,i));
}

void GridDrawingArea::set_center(int x, int y)
{
	centerx=x;
	centery=y;
	dirty=true;
	update();
}

void GridDrawingArea::set_zoom(int zoom)
{
	this->zoom=zoom;
//	cout << "zoom changed " << this->zoom << endl;	
	//refresh_pixmap();
	dirty=true;
	update();
}



void GridDrawingArea::on_realize()
{
	Gtk::DrawingArea::on_realize();
	//cout << "***********************************************Realized!\n";
	Glib::RefPtr<Gdk::Colormap> colormap = get_default_colormap();
	window = get_window();
	gc_=Gdk::GC::create(window);
	backgroundColor=Gdk::Color("black");
	gridColor.set_rgb(16384,16384,16384);
	scanColor.set_rgb(16384,65536,16384);
	centerColor=Gdk::Color("white");
	for (int i=0;i<256;i++)
	{
		colorgradient[i].set_rgb(0,i*256,0);
		colormap->alloc_color(colorgradient[i]);
	}
	colormap->alloc_color(backgroundColor);
	colormap->alloc_color(gridColor);
	colormap->alloc_color(centerColor);
	window->set_background(backgroundColor);
	gc_->set_background(backgroundColor);
//	gc_->set_foreground(foregroundColor);
	window->clear();
//	pixmap=Gdk::Pixmap::Create(get_window(),get_width(),get_height());
}


bool GridDrawingArea::on_expose_event(GdkEventExpose *event)
{
//	int i,j;
	//cout << "****************************************Exposed!"<<endl;
	if (height!=get_height() || width!=get_width() || dirty)
	{
		//Resize occurred
		height=get_height();
		width=get_width();
		
		pixmap=Gdk::Pixmap::create(window,width,height);
		refresh_pixmap();
		dirty=false;
		//cout << "Resize"<< endl;
	}
	window->draw_drawable(gc_,pixmap,0,0,0,0);
/*	if (eg)
	{
		//Glib::RefPtr<Gdk::Pixmap> pixmap=Gdk::Pixmap::create(
		for (i=200;i<600;i++)
			for (j=200;j<400;j++)
			{
				gc_->set_foreground(colorgradient[eg->at(0,j,i)]);
				window->draw_point(gc_,i,j);
			}
	}*/
	/*
	gc_->set_foreground(gridColor);
	
	int maxdim=((oldheight>oldwidth)?oldheight:oldwidth);
	
	for (i=0;i<maxdim;i+=pixelspercell)
	{
		window->draw_line(gc_,0,i,oldwidth,i);
		window->draw_line(gc_,i,0,i,oldheight);
	}
	*/
	/* Start plotting the cells */
/*	int noofxcells=oldwidth/pixelspercell;
	int noofycells=oldheight/pixelspercell;
	for (i=0;i<noofxcells;i++)
	{
		for (j=0;j<noofycells;j++)
		{
			int cellstartx,cellstarty;
			cellstartx=(i*pixelspercell)+1;
			int newj=noofycells-j;
			cellstarty=(j*pixelspercell)+1;
			int cellvalue=environment->get_data(centerx-(noofxcells/2)+i,centery-(noofycells/2)+newj);
			//cout <<cellvalue;
			if (cellvalue>0)
			{
			//	cout <<"foobar\n";
				// It's an obstacle 
				if (cellvalue>15) cellvalue=15;
					//cellvalue=15;
				gc_->set_foreground(colorgradient[cellvalue]);
				gc_->set_background(colorgradient[cellvalue]);
				window->draw_rectangle(gc_,true,cellstartx,cellstarty,pixelspercell-1,pixelspercell-1);
			}
			if (noofxcells/2==i && noofycells/2==newj)
			{
				gc_->set_foreground(centerColor);
				gc_->set_background(centerColor);
				window->draw_rectangle(gc_,true,cellstartx,cellstarty,pixelspercell-1,pixelspercell-1);
			}
		}
	}
	*/
	/*gc_->set_foreground(scanColor);
	gc_->set_background(scanColor);
	*/
	return true;
	
/*	gc_->set_foreground(foregroundColor);
	for (i=0;i<100;++i)
	{
		for (j=0;j<30;j++)
		{
			if (CGrid[i][j]!=0)
				window->draw_arc(gc_,true,points[i][j].get_x(),points[i][j].get_y(),CGrid[i][j]+2,CGrid[i][j]+2,1,23039);
		}
	}
	gc_->set_foreground(homeColor);
	window->draw_arc(gc_,true,oldwidth/2,oldheight-5,5,5,1,23039);
	if (curscanangle!=-1)
	{
		//window->draw_line(gc_,1,1,100,100);
	}
	if (destdir!=-1)
	{
		double radangle=(destdir*M_PI)/100.0;
		double radius=((oldwidth/2)>oldheight)?oldheight:(oldwidth/2);
		double len=500.0;
		int x = int(double(oldwidth/2)+(len*std::cos(radangle)));
		int y = int(double(oldheight)-(len*std::sin(radangle)));		
		window->draw_line(gc_,oldwidth/2,oldheight,x,y);
	}
//	return 0;*/
	
}

/*void GridDrawingArea::updatePoint(int cx, int cy)
{
	centerx=cx;
	centery=cy;
	Glib::RefPtr<Gdk::Window> window = get_window();
	window->invalidate_rect(Gdk::Rectangle(0,0,oldwidth,oldheight),false);
}*/
/*void GridDrawingArea::calcallpoints()
{
	int angle,distance;
	double radangle,radius,len;
	for (angle=0;angle<100;angle++)
	{
		for (distance=0;distance<30;distance++)
		{			
			radangle=(angle*M_PI)/100.0;
			radius=((oldwidth/2)>oldheight)?oldheight:(oldwidth/2);
			len=(double(distance)/30)*radius;
			points[angle][distance].set_x(int(double(oldwidth/2)+(len*std::cos(radangle))));
			points[angle][distance].set_y(int(double(oldheight)-(len*std::sin(radangle))));
		}
	}
}*/

/*bool GridDrawingArea::on_motion_notify_event(GdkEventMotion *event)
{
	statusbar->pop();
	std::ostringstream statusline;
	statusline << "Mouse moved over" << event->x << ", " << event->y;
	g_print("mousemove\n");
	statusbar->push(statusline.str());
	return 0;
}*/
