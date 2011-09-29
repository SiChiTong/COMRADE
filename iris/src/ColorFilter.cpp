#include "ColorFilter.h"

Comrade::IrisXT::ColorFilter::ColorFilter()
{
	red_tol=green_tol=blue_tol=0;
	filter_color.red=filter_color.green=filter_color.blue=0;

	std::cout<<"ColorFilter object created...\n";
}

void Comrade::IrisXT::ColorFilter::filter
(Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>* ip,
 Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>* op,
 int in_or_out)
{
	filter(ip,op,in_or_out,0,0,ip->maxx-1,ip->maxy-1);
}

bool Comrade::IrisXT::ColorFilter::within_range(RGB current)
{
	if (current.red>=filter_color.red-red_tol &&
	    current.red<=filter_color.red+red_tol &&
	    current.green>=filter_color.green-green_tol &&
	    current.green<=filter_color.green+green_tol &&
	    current.blue>=filter_color.blue-blue_tol &&
	    current.blue<=filter_color.blue+blue_tol)
	{
		return true;
	}
	
	else
	{
		return false;
	}			
}
void Comrade::IrisXT::ColorFilter::filter
(Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>* ip,
 Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>* op,
 int in_or_out,int x1,int y1,int x2,int y2)
{
	Comrade::IrisFoundation::RGB black={0,0,0};
	Comrade::IrisFoundation::RGB white={255,255,255};
	
	for (int yy=y1; yy<=y2; ++yy)
	{
		for (int xx=x1; xx<=x2; ++xx)
		{
			op->at(xx,yy)=black;
			
			Comrade::IrisFoundation::RGB& current=ip->at(xx,yy);
			
			if (current.red>=filter_color.red-red_tol &&
				current.red<=filter_color.red+red_tol &&
				current.green>=filter_color.green-green_tol &&
				current.green<=filter_color.green+green_tol &&
				current.blue>=filter_color.blue-blue_tol &&
				current.blue<=filter_color.blue+blue_tol)
			{
				if (in_or_out==FILTER_IN)
				{
					op->at(xx,yy)=white;
				}
			}
			
			else
			{
				if (in_or_out==FILTER_OUT)
				{
					op->at(xx,yy)=white;
				}
			}
		}
	}
}

Comrade::IrisXT::ColorFilter::~ColorFilter()
{}
