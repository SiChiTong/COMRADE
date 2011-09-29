#include "ObstacleAvoider.h"
#include "Constants.hh"
#include <iostream>

using namespace Comrade::ComradeRuntime;
using namespace Comrade::Corelib;
using namespace std;

#define OBSTACLE_AVOIDER_SEARCH_RADIUS 100
#define OBSTACLE_AVOIDER_SECTOR_ANGLE 10
#define OBSTACLE_AVOIDER_THRESHOLD_HIGH 50.0
#define OBSTACLE_AVOIDER_THRESHOLD_LOW 30.0
#define OBSTACLE_AVOIDER_DANGER_THRESHOLD 30.0
#define OBSTACLE_AVOIDER_CONSTANT_A 1.0
#define OBSTACLE_AVOIDER_CONSTANT_B (OBSTACLE_AVOIDER_CONSTANT_A/OBSTACLE_AVOIDER_SEARCH_RADIUS)

ObstacleAvoider::ObstacleAvoider(OdometryEngine *oe, EvidenceGrid<unsigned char> *eg)
{
	int x,y,i;
	lastvel=0;
	odometry_engine=oe;
	evidence_grid=eg;
	sectordraw.on_pixel_receive.connect(sigc::mem_fun(this,&ObstacleAvoider::check_point));
	noofsectors=360/OBSTACLE_AVOIDER_SECTOR_ANGLE;
	histogram=new double[noofsectors];
	density=new double[noofsectors];
	binhistogram=new bool[noofsectors];
	for (i=0;i<noofsectors;i++)
	{
		binhistogram[i]=true;
	}
	maskedhistogram=new bool[noofsectors];
	cout << "Generating VFH+ lookup table.." << endl;
	for (x=0;x<OBSTACLE_AVOIDER_SEARCH_RADIUS*2;x++)
	{		
		for (y=0;y<OBSTACLE_AVOIDER_SEARCH_RADIUS*2;y++)
		{
			int dx=x-OBSTACLE_AVOIDER_SEARCH_RADIUS;
			int dy=y-OBSTACLE_AVOIDER_SEARCH_RADIUS;
			double radius=sqrt(double(dy*dy)+double(dx*dx));
			if (radius<=OBSTACLE_AVOIDER_SEARCH_RADIUS)
				oalt[x][y].in_circle=true;
			else
			{
				oalt[x][y].in_circle=false;
				continue;
			}
			oalt[x][y].beta=(180.0/M_PI)*atan2(double(dy),double(dx));
			oalt[x][y].beta+=360.0;
			while (oalt[x][y].beta>=360.0) oalt[x][y].beta-=360.0;
			if (radius<=ROBOT_UNITS_RADIUS)
				oalt[x][y].gamma=90.0;
			else
				oalt[x][y].gamma=(180.0/M_PI)*asin(ROBOT_UNITS_RADIUS/radius);
			oalt[x][y].radiusfactor=OBSTACLE_AVOIDER_CONSTANT_A-(OBSTACLE_AVOIDER_CONSTANT_B*radius);
			double sa=oalt[x][y].beta-oalt[x][y].gamma;
			double ea=oalt[x][y].beta+oalt[x][y].gamma;
			oalt[x][y].startsector=int((sa+(OBSTACLE_AVOIDER_SECTOR_ANGLE/2.0))/OBSTACLE_AVOIDER_SECTOR_ANGLE);
			oalt[x][y].endsector=int((ea+(OBSTACLE_AVOIDER_SECTOR_ANGLE/2.0))/OBSTACLE_AVOIDER_SECTOR_ANGLE);
/*			cout << dx << " "
				<< dy << " "
				<< radius << " " 
				<< oalt[x][y].beta << " "
				<< oalt[x][y].gamma << " "
				<< oalt[x][y].radiusfactor << " "
				<< sa << " "
				<< ea << " "
				<< oalt[x][y].startsector << " "
				<< oalt[x][y].endsector << endl;*/
		}
	}
	cout << "Done!"<< endl;
}

ObstacleAvoider::~ObstacleAvoider()
{
	delete[] histogram;
	delete[] binhistogram;
	delete[] maskedhistogram;
	delete[] density;
}

void ObstacleAvoider::check_point(std::pair<int,int> point,int posz, int radius,
			int reading, double angle, bool is_obstacle,
			std::pair<int,int> offset)
{
	int gridvalue=evidence_grid->at(0,point.second,point.first);
	int gridprob=gridvalue&0x07;
	//cout << gridprob << " " << radius << " " << reading << endl;
	density[cursector]+=double(gridprob)*(double(reading-radius)/double(reading));
	//cout << "point: " << point.first << " " << point.second << " " << density[cursector] << endl;
}

/*std::pair<double,double> ObstacleAvoider::get_speeds()
{
	std::pair<int, int> speeds;
	bool in_chunk=false;
	int chunkstart=0;
	int chunkend=-1;
	int maxchunksize=0;
	int maxchunkstart, maxchunkend;
	int chunksize=0;

	int x,y;
	int i,j;
	for (i=0;i<noofsectors;i++)
		histogram[i]=0.0;
	for (x=0;x<OBSTACLE_AVOIDER_SEARCH_RADIUS*2;x++)
	{		
		for (y=0;y<OBSTACLE_AVOIDER_SEARCH_RADIUS*2;y++)
		{
			if (oalt[x][y].in_circle)
			{
				int gridvalue=evidence_grid->at(0,
					x+odometry_engine->get_x()-OBSTACLE_AVOIDER_SEARCH_RADIUS,
					y+odometry_engine->get_y()-OBSTACLE_AVOIDER_SEARCH_RADIUS);
				int gridprob=gridvalue&0x07;
				double m=(gridprob*gridprob)*oalt[x][y].radiusfactor;
				for (j=oalt[x][y].startsector;j<=oalt[x][y].endsector;j++)
				{
					int sec=j%noofsectors;
					histogram[sec]+=m;
				}
			}
		}
	}
	for (i=0;i<noofsectors;i++)
	{
		if (histogram[i]>OBSTACLE_AVOIDER_THRESHOLD_HIGH) binhistogram[i]=true;
		if (histogram[i]<OBSTACLE_AVOIDER_THRESHOLD_LOW) binhistogram[i]=false;
	}
	//TODO: 
	
	for (i=0;i<noofsectors;i++)
	{
		double sa=double(i*OBSTACLE_AVOIDER_SECTOR_ANGLE)-(OBSTACLE_AVOIDER_SECTOR_ANGLE/2.0);
		double ea=sa+OBSTACLE_AVOIDER_SECTOR_ANGLE;
		cursector=i;
		cout << "Sector " << i << ": " << histogram[i] << " " << binhistogram[i] << endl;
		if (in_chunk)
		{
			chunksize++;
			if (binhistogram[i])
			{
				chunkend=i-1;
				if (chunksize>maxchunksize)
				{
					maxchunkstart=chunkstart;
					maxchunkend=chunkend;
					maxchunksize=chunksize;
				}
				in_chunk=false;
				chunksize=0;
			}
		}
		else
		{
			if (!binhistogram[i])
			{
				in_chunk=true;
				chunkstart=i;
			}
		}
	}
	if (in_chunk)
	{
		i=0;
		while (!binhistogram[i] && i<noofsectors-1)
		{
			chunksize++;
			chunkend=i;
			++i;
		}
	}
	if (chunksize>maxchunksize)
	{
		maxchunkstart=chunkstart;
		maxchunkend=chunkend;
		
		maxchunksize=chunksize;
	}
	if (maxchunksize!=0)
	{
		cout << "Largest chunk = " << maxchunkstart << " to " << maxchunkend << endl;
		if (maxchunkend<maxchunkstart) maxchunkend+=noofsectors;
		double dest_angle=odometry_engine->get_orientation()+(maxchunkstart+(maxchunksize/2))*OBSTACLE_AVOIDER_SECTOR_ANGLE;
	//	double avgdensity=totaldensity/noofsectors;
		// Now move towards mindensitynumber
		//double dest_angle=odometry_engine->get_orientation()+((mindensitynumber+0.5)*OBSTACLE_AVOIDER_SECTOR_ANGLE);
		cout << dest_angle << endl;
		speeds.first=dest_angle;
		speeds.second=100.0;
	}
	else
	{
		speeds.first=0;
		speeds.second=0;
	}
	return speeds;
}*/

std::pair<double,double> ObstacleAvoider::get_speeds()
{
	std::pair<int, int> speeds;
	bool in_chunk=false;
	int chunkstart=0;
	int chunkend=-1;
	int maxchunksize=0;
	int maxchunkstart, maxchunkend;
	int chunksize=0;
	double mindensity=999999999999999.0;
	int mindensitynumber=0;
	double totaldensity=0.0;
	//cout << "get_speeds!" << endl;
	//cout << "No of sectors = " << noofsectors << endl;
	int i;
	for (i=0;i<noofsectors;i++)
	{
		density[i]=0.0;
		double sa=double(i*OBSTACLE_AVOIDER_SECTOR_ANGLE);
		double ea=sa+OBSTACLE_AVOIDER_SECTOR_ANGLE;
		cursector=i;
		sectordraw.draw_sector(odometry_engine->get_x(),odometry_engine->get_y(),
				OBSTACLE_AVOIDER_SEARCH_RADIUS,sa,ea);
//		totaldensity+=density[i];
		cout << "Sector " << i << ": " << density[i] << endl;
		if (in_chunk)
		{
			chunksize++;
			if (density[i]>OBSTACLE_AVOIDER_DANGER_THRESHOLD)
			{
				chunkend=i-1;
				if (chunksize>maxchunksize)
				{
					maxchunkstart=chunkstart;
					maxchunkend=chunkend;
					maxchunksize=chunksize;
				}
				in_chunk=false;
				chunksize=0;
			}
		}
		else
		{
			if (density[i]<OBSTACLE_AVOIDER_DANGER_THRESHOLD)
			{
				in_chunk=true;
				chunkstart=i;
			}
		}
	}
	if (in_chunk)
	{
		i=0;
		while (density[i]<OBSTACLE_AVOIDER_DANGER_THRESHOLD && i<31)
		{
			chunksize++;
			chunkend=i;
			++i;
		}
	}
	if (chunksize>maxchunksize)
	{
		maxchunkstart=chunkstart;
		maxchunkend=chunkend;
		
		maxchunksize=chunksize;
	}
	if (maxchunksize!=0)
	{
		cout << "Largest chunk = " << maxchunkstart << " to " << maxchunkend << endl;
		if (maxchunkend<maxchunkstart) maxchunkend+=36;
		double dest_angle=odometry_engine->get_orientation()+(maxchunkstart+(maxchunksize/2))*OBSTACLE_AVOIDER_SECTOR_ANGLE;
	//	double avgdensity=totaldensity/noofsectors;
		// Now move towards mindensitynumber
		//double dest_angle=odometry_engine->get_orientation()+((mindensitynumber+0.5)*OBSTACLE_AVOIDER_SECTOR_ANGLE);
		cout << dest_angle << endl;
		speeds.first=dest_angle;
		speeds.second=100.0;
	}
	else
	{
		speeds.first=0;
		speeds.second=0;
	}
	return speeds;
}
