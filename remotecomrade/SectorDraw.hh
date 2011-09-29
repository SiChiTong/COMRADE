#ifndef SECTORDRAW_HH
#define SECTORDRAW_HH

#include "sigc++/sigc++.h"
#include <utility>

namespace Comrade
{

namespace Corelib
{

struct pixel_
{
	int x;
	int y;
};
	
class SectorDraw
{
	public:
		SectorDraw();
		~SectorDraw();
		void draw_sector(int x, int y, int radius, double start_angle, double end_angle);
		sigc::signal<void,std::pair<int,int>,int,int,int,double,bool,std::pair<int,int> > on_pixel_receive;

	private:
		void calc_circle(int radius);
		void draw_small_sector(int x, int y, int radius, double start_angle, double end_angle);
		void put_pixel(int x, int y, int offsetx, int offsety, int octant, bool obstacle_present);
		pixel_ *circle_pixels;
		int no_of_circle_pixels;
		int reading;
		double angle;
};

}
}

#endif
