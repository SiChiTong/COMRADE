#include "LineFollower.h"
#include <iostream>

using namespace std;
using namespace Comrade::IrisFoundation;
using namespace Comrade::IrisXT;

Comrade::ComradeRuntime::LineFollower::LineFollower()
{
	detor=new StraightLineDetector(320+240,180,1);
	filter_buffer=new Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>(320,240); // Replace with imported constants later...
}

void Comrade::ComradeRuntime::LineFollower::assign_ip_buffer
(Comrade::IrisFoundation::Buffer
<Comrade::IrisFoundation::RGB>* buf)
{
	ip_buffer=buf;
}

void Comrade::ComradeRuntime::LineFollower::assign_filter_buffer
(Comrade::IrisFoundation::Buffer
<Comrade::IrisFoundation::RGB>* buf)
{
	filter_buffer=buf;
}

void Comrade::ComradeRuntime::LineFollower::set_start_scanline(int scanln)
{
	start_scanline=scanln;
}

void Comrade::ComradeRuntime::LineFollower::set_filter_color
(Comrade::IrisFoundation::RGB color)
{
	filter_color=color;	
}

std::pair<int,int> Comrade::ComradeRuntime::LineFollower::detect_line()
{
   BitmapStreamer left,right;
   RGB_BufferManager someman,ipman;
   //EdgeDetectorManager eddie;
   
   someman.assign_buffer(filter_buffer);
   ipman.assign_buffer(ip_buffer);
   
   
   left.byte_reader.reader.open("/home/adam/cvs/comrade-main/src/Picture.bmp",ios_base::in|ios_base::out);
   left.byte_reader.writer.open("/home/adam/cvs/comrade-main/src/Picture.bmp",ios_base::in|ios_base::out);
   right.byte_reader.reader.open("/home/adam/cvs/comrade-main/src/Blank.bmp",ios_base::in|ios_base::out);
   right.byte_reader.writer.open("/home/adam/cvs/comrade-main/src/Blank.bmp",ios_base::in|ios_base::out);
   
   long int** array;
   
   
   RGB save={5,5,5};
   
   cfilter->filter_color=save;
   cfilter->red_tol=5;
   cfilter->green_tol=5;
   cfilter->blue_tol=5;

   RGB white={252,252,252};
   RGB black={0,0,0};

   cfilter->filter(ip_buffer,filter_buffer,FILTER_IN,0,130,319,239);
   //eddie.Canny(filter_buffer,ip_buffer,.25);
   
   someman.copy_to_image(&right);
   ipman.copy_to_image(&left);

   detor->transform(filter_buffer);
   
   array=detor->return_array();

   long int max=array[0][0];
   int maxperp=0,maxtheta=0;
   
   for (int y=0; y<=detor->return_rows()-1; y++)
   {
   	for (int x=0; x<=detor->return_cols()-1; x++)
      	{
   		if (array[y][x]>max)
		{
			max=array[y][x];
			maxperp=y;
			maxtheta=x;
		}
	}
   }

   maxtheta*=detor->return_angle_increment();
   
   int intersect_x=(maxperp-(239-start_scanline)*sin(deg_to_rad(maxtheta)))
   		   /cos(deg_to_rad(maxtheta));
   
   set_start_scanline(130);
   
   if (intersect_x>319)
   {
	   intersect_x=319;
	   //set_start_scanline(260);
   }

   if (intersect_x<0)
   {
	   intersect_x=0;
	   //set_start_scanline(260);
   }
   
   cout<<maxperp<<" "<<maxtheta<<" and intersection at x="<<intersect_x<<endl;
   
   right.byte_reader.reader.close();
   right.byte_reader.writer.close();
   left.byte_reader.reader.close();
   left.byte_reader.writer.close();

    /*for (int i=0; i<=detor->return_rows()-1; ++i)
    {
        delete[] array[i];
    }

    delete[] array;*/
   
   // Above bad hack has been superseded using a more rigid
   // Hough Transform Engine.
    
    std::pair<int,int> result;
    result.first=intersect_x;
    result.second=90-maxtheta;

    return result;
}

Comrade::ComradeRuntime::LineFollower::~LineFollower()
{
	delete detor;
	delete filter_buffer;
}
