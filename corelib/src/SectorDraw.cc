#include "SectorDraw.hh"
#include "Constants.hh"
#include <cmath>
#include <iostream>

using namespace Comrade::Corelib;
using namespace std;

SectorDraw::SectorDraw()
{
	circle_pixels=new pixel_[SONAR_MAX_RANGE_READING*2];
}

SectorDraw::~SectorDraw()
{
	delete[] circle_pixels;
}

void SectorDraw::calc_circle(int radius)
{
	int d,x,y,ptr=0;
	d=3-(2*radius);
	x=0;
	y=radius;
	do
	{
		circle_pixels[ptr].x=x;
		circle_pixels[ptr].y=y;
		ptr++;
		if (d<0) d=d+(4*x)+6;
		else
		{
			d=d+4*(x-y)+10;
			y=y-1;
		}
		x++;
	}
	while (x<y);
	no_of_circle_pixels=ptr;
}

void SectorDraw::draw_sector(int x, int y, int radius, double start_angle, double end_angle) // degrees
{
//	double temp;
	int i;
	reading=radius;
	// Calculate the Bresenham circle coordinates (45 degrees)
	
	calc_circle(radius);

	// normalize the angles
	
/*	while (start_angle>360.0) start_angle-=360.0;
	while (start_angle<0.0) start_angle+=360.0;
	while (end_angle>360.0) end_angle-=360.0;
	while (end_angle<0.0) end_angle+=360.0;

	// put the smaller angle first
	
	if (start_angle>end_angle)
	{
		temp=start_angle;
		start_angle=end_angle;
		end_angle=temp;
	}
*/	
	// Split up the given sector into smaller sectors, max of 45
	// degrees each
	
	int startno=int(start_angle/45.0);
	int endno=int(end_angle/45.0);
	double segend;
	double segstart=startno*45.0;
	double sa,ea;
	for (i=startno; i<=endno; i++)
	{
		segend=(i+1)*45.0;
		if (start_angle>segstart) sa=start_angle;
			else sa=segstart;
		if (end_angle<segend) ea=end_angle;
			else ea=segend;
		segstart=segend;
		// Draw the small sector
		if (ea!=sa) draw_small_sector(x,y,radius,sa,ea);
	}
}

void SectorDraw::draw_small_sector(int x, int y, int radius, double start_angle, double end_angle)
{
	// Now comes the main problem...	
	int octant=0;
	bool reverse_angles=false;
	double temp;
	
	// Transpose the sector to be drawn to the first octant.
	
	while (start_angle>=45.0)
	{
		octant++;
		start_angle-=45.0;
		end_angle-=45.0;
	}
	if (octant%2)
	{
		reverse_angles=true;
		temp=start_angle;
		start_angle=45.0-end_angle;
		end_angle=45.0-temp;
	}

	octant%=8;
	
	// In this octant, the "anchor line" is the start_angle line,
	// We need to calculate x,y endpoints of the start_angle and
	// end_angle line
	
	double sarad=start_angle*(M_PI/180.0);
	double earad=end_angle*(M_PI/180.0);

	int slinex=int(radius*sin(sarad));
	int sliney=int(radius*cos(sarad));
	int elinex=int(radius*sin(earad));
	int eliney=int(radius*cos(earad));
	
	int salx=0, saly=0;
	int ealx=0, ealy=0;

	int sd = (2*slinex)-sliney;
	int ed = (2*elinex)-eliney;

	int i;

	// Paint lines from the anchor line to the other line first
	
	for (saly=0;saly<=eliney;saly++,ealy++)
	{

		for (i=salx;i<=ealx;i++)
		{
			put_pixel(x,y,i,-saly,octant,false);
		}
		
		if (sd<0) sd=sd+2*slinex;
		else
		{
			sd=sd+2*(slinex-sliney);
			salx++;
		}
		if (ed<0) ed=ed+2*elinex;
		else
		{
			ed=ed+2*(elinex-eliney);
			ealx++;
		}
	}

	// Now paint lines from anchor line to arc
	
	int arc_index=no_of_circle_pixels-1;
	
	while (circle_pixels[arc_index].y<saly) arc_index--;

	int apx2,apx1;
	
	for (;saly<=sliney;saly++)
	{
		apx2=circle_pixels[arc_index].x;
		while (circle_pixels[arc_index].y==saly) arc_index--;
		apx1=circle_pixels[arc_index+1].x;
		if (apx1<slinex) apx1=slinex;
		if (apx2>elinex) apx2=elinex;
		//Find the corresponding arc coordinate for this y
		for (i=salx;i<apx1;i++)
		{
			put_pixel(x,y,i,-saly,octant,false);
		}
		for (i=apx1;i<=apx2;i++)
		{
			put_pixel(x,y,i,-saly,octant,true);
		}
		if (sd<0) sd=sd+2*slinex;
		else
		{
			sd=sd+2*(slinex-sliney);
			salx++;
		}

	}
	if (arc_index>0)
	{
		apx2=circle_pixels[arc_index].x;
		while (circle_pixels[arc_index].y==saly) arc_index--;
		apx1=circle_pixels[arc_index+1].x;
		if (apx1<slinex) apx1=slinex;
		for (i=apx1;i<=apx2;i++)
		{
			put_pixel(x,y,i,-saly,octant,true);
		}
	}
	
}

void SectorDraw::put_pixel(int x, int y, int offsetx, int offsety, int octant, bool obstacle_present)
{
	int ox=offsetx,oy=offsety;
	switch (octant)
	{
		case 1: oy=-offsetx;
			ox=-offsety;
			break;
		case 2: oy=offsetx;
			ox=-offsety;
			break;
		case 3: oy=-offsety;
			ox=offsetx;
			break;
		case 4: oy=-offsety;
			ox=-offsetx;
			break;
		case 5: oy=offsetx;
			ox=offsety;
			break;
		case 6: oy=-offsetx;
			ox=offsety;
			break;
		case 7: oy=offsety;
			ox=-offsetx;
			break;
	}
	int r=int(sqrt(double(ox*ox+oy*oy)));
	std::pair<int,int> p1(x+ox,y+oy);
	std::pair<int,int> p2(ox,oy);
	on_pixel_receive(p1,0,r,reading,angle,obstacle_present,p2);
}
