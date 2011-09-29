#ifndef GLDRAWINGAREA
#define GLDRAWINGAREA

#include <gtkmm.h>
#include <gtkglmm.h>
#include "adam.h"
#include "EvidenceGrid.hh"
#include "view3d.hh"
//#include "gui_shared_data.h"
#include <libglademm/xml.h>


extern unsigned char map[800][600];

class GL3DScene : public Gtk::DrawingArea, public Gtk::GL::Widget, public Comrade::Corelib::Singleton<GL3DScene>
{
	public:
		//GL3DScene(BaseObjectType *cobject, const Glib::RefPtr<Gnome::Glade::Xml>& refGlade);
		GL3DScene();
		virtual ~GL3DScene();
		//virtual void update_point(int x, int y, unsigned char color);
		virtual void redraw(void);
		void set_evidence_grid(Comrade::Corelib::EvidenceGrid<unsigned char> *grid);
		void update(double x, double y, double robot_orientation);
	protected:
		virtual void on_realize();
		virtual bool on_configure_event(GdkEventConfigure* event);
		virtual bool on_expose_event(GdkEventExpose* event);
		virtual bool on_button_press_event(GdkEventButton* event);
		virtual bool on_button_release_event(GdkEventButton* event);
		virtual bool on_motion_notify_event(GdkEventMotion* event);	
		void draw(void);
	
		float anglex, angley, xdist, ydist, zdist;
		float orientationx, orientationy;
	
		//camera view
		float camerax, cameray, cameraz;
		float camera_ax, camera_ay;
	private:
		//Robot_Dimensions dim;
		//GLUquadricObj *quadric;
		Adam adam;
		int map_x, map_y;
		Glib::RefPtr<Gnome::Glade::Xml> m_refGlade;
		int buttonpressed;
		int buttonstartx, buttonstarty;
		Comrade::Corelib::EvidenceGrid<unsigned char> *mapgrid;
	
		object3d *obj;
	
};

#endif
