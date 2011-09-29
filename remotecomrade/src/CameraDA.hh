#ifndef CAMERA_DRAWINGAREA
#define CAMERA_DRAWINGAREA

#include <gtkmm.h>
#include <gtkglmm.h>
//#include "gui_shared_data.h"
#include "Singleton.hh"
#include <libglademm/xml.h>
#include "bitmap.h"
#include <string.h>

class cameraview : public Gtk::DrawingArea, public Gtk::GL::Widget, public Comrade::Corelib::Singleton<cameraview>
{
	public:
		cameraview();
		~cameraview();
		//void setimage(unsigned char* img_ptr);
		void setimage(char* filename);
		bool update();
		void draw();
	protected:
		virtual void on_realize();
		virtual bool on_configure_event(GdkEventConfigure* event);
		virtual bool on_expose_event(GdkEventExpose* event);

	private:
		BITMAPINFO *BitmapInfo; /* Bitmap information */
		GLubyte    *BitmapBits; /* Bitmap data */
		std::string imagefile;
		void drawimage(void);

};

#endif
