#include "HoughTransform.h"

using namespace Comrade::IrisFoundation;

Comrade::IrisXT::HoughTransformObject::HoughTransformObject()
{}

Comrade::IrisXT::HoughTransformObject::~HoughTransformObject()
{}

long int** Comrade::IrisXT::StraightLineDetector::return_array()
{
	return param_array;
}

/*Comrade::IrisXT::StraightLineDetector::StraightLineDetector()
{
	//allocate(10,180); //DUMMY
}*/

int Comrade::IrisXT::StraightLineDetector::return_rows()
{
    return rows;
}

int Comrade::IrisXT::StraightLineDetector::return_cols()
{
    return cols;
}

int Comrade::IrisXT::StraightLineDetector::return_angle_increment()
{
	return angle_increment;
}

void Comrade::IrisXT::StraightLineDetector::allocate(int perp,int theta)
{
    cout<<"StraightLineDetector object created...\n";

    rows=perp;
    cols=theta;

    param_array=new long int*[rows];

    for (int i=0; i<=rows-1; i++)
    {
        param_array[i]=new long int[cols];
    }
    
    for (int i=0; i<=rows-1; i++)
    {
	    for (int j=0; j<=cols-1; j++)
	    {
		    param_array[i][j]=0;
	    }
    }
}

void Comrade::IrisXT::StraightLineDetector::clear_param_array()
{
	for (int i=0; i<=rows-1; ++i)
	{
		for (int j=0; j<=cols-1; ++j)
		{
			param_array[i][j]=0;
		}
	}
}

void Comrade::IrisXT::StraightLineDetector::deallocate()
{
    for (int i=0; i<=rows-1; i++)
    {
        delete[] param_array[i];
    }

    delete[] param_array;
}

Comrade::IrisXT::StraightLineDetector::StraightLineDetector(int perp_levels,int theta_levels,int ang_inc)
{
    	angle_increment=ang_inc;
	allocate(perp_levels,theta_levels/angle_increment);
}

Comrade::IrisXT::StraightLineDetector::StraightLineDetector(Buffer<RGB>* buf,int theta_levels,int ang_inc)
{
    	angle_increment=ang_inc;
	int l=buf->maxx;
    	int b=buf->maxy;
    
    	int h=abs(l)+abs(b);
	allocate(h,theta_levels/angle_increment);
}

/*void Comrade::IrisXT::StraightLineDetector::set_angle_increment(int inc)
{
	angle_increment=inc;
}*/

void Comrade::IrisXT::StraightLineDetector::set_param(int perp_levels,int theta_levels)
{
    deallocate();
    allocate(perp_levels,theta_levels);
}

void Comrade::IrisXT::StraightLineDetector::transform(Buffer<RGB>* ip,
						      int x1,int y1,
						      int x2,int y2)
{
	//deallocate();
    	int l=x2-x1;
    	int b=y2-y1;
    
	int h=abs(l)+abs(b);
    //allocate(h,180/angle_increment);
	clear_param_array();
	
	cout<<"Doing Hough Transform now...\n";
	
    for (int y=y1; y<=y2; ++y)
    {
	    for (int x=x1; x<=x2; ++x)
	    {
			RGB color=ip->at(x,y);
			
		    if (color.red==255 && color.green==255 && color.blue==255)
		    {
			    for (int i=0; i<=cols-1; ++i)
				{	    
					double ang=deg_to_rad(angle_increment*i);
					double perp=x*cos(ang)+(ip->maxy-1-y)*sin(ang);
				
					if (perp>=0 && perp<=h-1)
					{
						param_array[static_cast<int>(perp)][i]++;
					}
				}
			}
		}
	}

	//deallocate();
}

void Comrade::IrisXT::StraightLineDetector::transform(Buffer<RGB>* ip)
{
	transform(ip,0,0,ip->maxx-1,ip->maxy-1);
}

Comrade::IrisXT::StraightLineDetector::~StraightLineDetector()
{
    deallocate();
}

Comrade::IrisXT::BasicShapeDetector::BasicShapeDetector()
{
	change_sz=0;
    samples=0;
}
			
void Comrade::IrisXT::BasicShapeDetector::build_shape_table
(Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>* shape_screen,
 int cx,int cy,int num_samples)
{
	using namespace Comrade::IrisFoundation;
 	
    samples=num_samples;
    ShapeSampler sampler;
	
	Buffer<RGB> op_buf(*shape_screen);
	
	edman.Canny(shape_screen,&op_buf,.009);

	sampler.sample(&op_buf,&shape_table,num_samples,cx,cy);
}

void Comrade::IrisXT::BasicShapeDetector::transform
	      (Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>* ip)
{
	using namespace Comrade::IrisFoundation;
	
	screen_array.reallocate(ip->maxx,ip->maxy);
  	
   	BufferManager<int> man;
 	
  	man.assign_buffer(&screen_array);
  	man.fill(0);
  	Buffer<RGB> op(*ip);
	edman.Canny(ip,&op,.25);
	
	int inc=360/shape_table.size();
 	
  	for (int yy=0; yy<=ip->maxy-1; ++yy)
	{
		for (int xx=0; xx<=ip->maxx-1; ++xx)
		{
			RGB color=op.at(xx,yy);
			
			if (color.red!=255 ||
				color.green!=255 ||
				color.blue!=255)
			{
   			    continue;
            }
            
            int cur=0;
			
   			for (unsigned int i=0; i<=shape_table.size()-1; ++i)
			{
				int actual=cur-180;
				//actual=(actual<0)?(actual+360):actual;
    
                int rx=xx+static_cast<int>((shape_table[i]+change_sz)*cos(deg_to_rad(actual)));
				int ry=yy-static_cast<int>((shape_table[i]+change_sz)*sin(deg_to_rad(actual)));
	
    			screen_array.at(rx,ry)++;
       			cur+=inc;
    		}
		}
	}
}

Comrade::IrisFoundation::Buffer<int>* Comrade::IrisXT::BasicShapeDetector::return_array()
{
	return (&screen_array);
}

Comrade::IrisXT::BasicShapeDetector::~BasicShapeDetector()
{

}

Comrade::IrisXT::CircleDetector::CircleDetector(int width,int height,int minrad,int maxrad)
{
	allocate(width,height,minrad,maxrad);
}
	
void Comrade::IrisXT::CircleDetector::allocate(int width,int height,
						int minrad,int maxrad)
{
	rows=height;
	cols=width;
	min_radius=minrad;
	max_radius=maxrad;
	
	radius_range=max_radius-min_radius+1;
	
	param_array=new long int**[radius_range];
	
	for (int i=0; i<=radius_range-1; ++i)
	{
		param_array[i]=new long int*[rows];
		
		for (int j=0; j<=rows-1; ++j)
		{
			param_array[i][j]=new long int[cols];
		}
	}
}

void Comrade::IrisXT::CircleDetector::deallocate()
{
	for (int i=0; i<=radius_range-1; ++i)
	{		
		for (int j=0; j<=rows-1; ++j)
		{
			delete[] param_array[i][j];
		}
	
		delete[] param_array[i];
	}
	
	delete[] param_array;
}

void Comrade::IrisXT::CircleDetector::transform(Buffer<RGB>* ip,
	       					int x1,int y1,
	       					int x2,int y2)
{
	std::cout<<"Starting circle detection NOW...\n";
	
	for (int yy=y1; yy<=y2; ++yy)
	{
		for (int xx=x1; xx<=x2; ++xx)
		{
			RGB color=ip->at(xx,yy);
			
			if (cfilter.within_range(color)==true)
			{
				for (int radius=min_radius; radius<=max_radius; ++radius)
				{
					for (int ang=0; ang<=359; ++ang)
					{
						int x=xx+radius*cos(deg_to_rad(ang));
						int y=yy-radius*sin(deg_to_rad(ang));
						
						//std::cout<<x<<" "<<y<<std::endl;
						
						if ((x>=0 && x<=ip->maxx-1) && 
						    (y>=0 && y<=ip->maxy-1))
						{
							++param_array[radius-min_radius][y][x];
						}
					}
				}
			}
		}
	}
}

void Comrade::IrisXT::CircleDetector::transform(Buffer<RGB>* ip)
{
	transform(ip,0,0,ip->maxx-1,ip->maxy-1);
}

Comrade::IrisXT::CircleDetector::~CircleDetector()
{
	deallocate();
}
