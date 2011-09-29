#include "EvidenceGrid.hh"
//#include "ExecutionEngine.h"

using namespace Comrade::Corelib;

void EvidenceGrid<unsigned char>::allocate(int z,int y,int x)
{
	zsz=z;
	ysz=y;
	xsz=x;
		
	grid=new unsigned char**[zsz];
		
	for (int i=0; i<=zsz-1; ++i)
	{
		grid[i]=new unsigned char*[ysz];
			
		for (int j=0; j<=ysz-1;++j)
		{
			grid[i][j]=new unsigned char[xsz];
		}
	}
}

void EvidenceGrid<unsigned char>::deallocate()
{
	for (int i=0; i<=zsz-1; ++i)
	{			
		for (int j=0; j<=ysz-1;++j)
		{
			delete[] grid[i][j];
		}
		
		delete[] grid[i];
	}
	
	delete[] grid;
}

void EvidenceGrid<unsigned char>::set_sonar_age(int age)
{
	sonar_age=age;
}

EvidenceGrid<unsigned char>::EvidenceGrid(int z,int y,int x)
{
	allocate(z,y,x);
	sonar_age=0;
}

void EvidenceGrid<unsigned char>::update_from_sonar(map_update& mu, SectorDraw &sectordraw)
{
	double b=mu.orientation+SONAR_START_ANGLE+(mu.sonar_angle*SONAR_ANGLE_SIZE);
	sonar_age++;
	if(sonar_age==32) sonar_age=1;
	sectordraw.draw_sector(mu.x-ROBOT_UNITS_X,mu.y-ROBOT_UNITS_Y,mu.sonar_data[0],b-SONAR_HALF_BEAM_WIDTH,b+SONAR_HALF_BEAM_WIDTH);
	sectordraw.draw_sector(mu.x+ROBOT_UNITS_X,mu.y-ROBOT_UNITS_Y,mu.sonar_data[1],b+90.0-SONAR_HALF_BEAM_WIDTH,b+90.0+SONAR_HALF_BEAM_WIDTH);
	sectordraw.draw_sector(mu.x-ROBOT_UNITS_X,mu.y+ROBOT_UNITS_Y,mu.sonar_data[2],b+180.0-SONAR_HALF_BEAM_WIDTH,b+180.0+SONAR_HALF_BEAM_WIDTH);
	sectordraw.draw_sector(mu.x+ROBOT_UNITS_X,mu.y+ROBOT_UNITS_Y,mu.sonar_data[3],b+270.0-SONAR_HALF_BEAM_WIDTH,b+270.0+SONAR_HALF_BEAM_WIDTH);
}

unsigned char EvidenceGrid<unsigned char>::get_probability(int z, int y, int x)
{
	return ((grid[z][y][x]&0x07)<<5);
}

void EvidenceGrid<unsigned char>::update_vfh
(std::pair<int,int> point,int posz,
int radius, int reading, double angle, bool is_obstacle,std::pair<int,int> offset)
{
	int posx=point.first;
	int posy=point.second;
	
//	int plus=64,minus=64;
//	double rate=.07;
	
	unsigned char oldval=grid[posz][posy][posx];
	unsigned char age=(oldval&0xF8)>>3;
	unsigned char prob=(oldval&0x07);
	unsigned char newprob;
	
	int curage=sonar_age;//=Comrade::ComradeRuntime::ExecutionEngine::get_singleton_ptr()->sonar_age;
	int agediff=curage-age;
	if (agediff<=0) agediff+=31;
	if (age==0) agediff=31;
	
	if (!is_obstacle)
	{	
		if (prob>1) newprob=prob-2;
			else newprob=0;
	}
	else
	{
		if (prob==0 && agediff<13)
		{
			newprob=0;
		}
		else
		{
			if (prob<6) newprob=prob+2;
			else newprob=7;
		}
	}

	
	grid[posz][posy][posx]=(curage<<3)+newprob;
	on_update(posx,posy,newprob?(newprob*32)+31:0);
}

void EvidenceGrid<unsigned char>::update
(std::pair<int,int> point,int posz,
int radius, int reading, double angle, bool is_obstacle,std::pair<int,int> offset)
{
	double alpha=((180/M_PI)*atan2(double(offset.first),-double(offset.second)));
	int posx=point.first;
	int posy=point.second;
	alpha+=360.0;
	while(alpha>=360.0) alpha-=360.0;
	while(angle>=360.0) angle-=360.0;
	alpha=fabs(angle-alpha);
	if (alpha>180.0 && alpha<=360.0) alpha=360.0-alpha;
	if (alpha>15.0 && alpha<180.0) alpha=15.0;
	
	/*if (is_obstacle==true)
	{
		grid[posz][posy][posx]=static_cast<unsigned char>(255);
		
		return;
	}
	
	double change=(50./reading)*radius;

	if (grid[posz][posy][posx]-change<0)
	{
		grid[posz][posy][posx]=static_cast<unsigned char>(0);
	}
	
	else
	{
		grid[posz][posy][posx]-=static_cast<unsigned char>(change);
	}*/
	
	
	/*double prev_prob=grid[posz][posy][posx]/255.;
	
	double p_s_given_occ=.5*((SONAR_MAX_RANGE_READING-radius)/
							  SONAR_MAX_RANGE_READING
										+
							 (SONAR_HALF_BEAM_WIDTH-alpha)/
							  SONAR_HALF_BEAM_WIDTH);
	
	if (is_obstacle==true)
	{
		//Scale factor ??
		//grid[posz][posy][posx]=255;
		//return;
		//p_s_given_occ*=.98;
	}
	
	double new_prob=p_s_given_occ*(prev_prob);
	double divisor=p_s_given_occ*prev_prob+
				  (1-p_s_given_occ)*(1-prev_prob);
	
	new_prob/=divisor;
	
	grid[posz][posy][posx]=static_cast<unsigned char>(new_prob*255);*/
		
	int plus=20,minus=50;
	double rate=.07;
	
	if (is_obstacle==true)
	{	
		if (grid[posz][posy][posx]<255-plus)
		{
			double time=-std::log(1-1.*grid[posz][posy][posx]/SONAR_MAX_RANGE_READING)/rate;
			//grid[posz][posy][posx]+=plus*(1-alpha/SONAR_HALF_BEAM_WIDTH);
			grid[posz][posy][posx]=SONAR_MAX_RANGE_READING*(1-std::exp(-rate*(time+3)));
			
			if (reading>0)
			{
				grid[posz][posy][posx]*=(1-radius/reading);
			}
		}
		
		else grid[posz][posy][posx]=255;
	}
	
	else
	{
		if (grid[posz][posy][posx]>minus)
		{
			double time=-std::log(1-1.*grid[posz][posy][posx]/SONAR_MAX_RANGE_READING)/rate;
			grid[posz][posy][posx]=SONAR_MAX_RANGE_READING*(1-std::exp(-rate*(time-1)));
			//grid[posz][posy][posx]-=minus*(1-alpha/SONAR_HALF_BEAM_WIDTH);
			
			if (reading>0)
			{
				grid[posz][posy][posx]*=(1-radius/reading);
			}
		}
		
		else grid[posz][posy][posx]=0;
		
	}
	on_update(posx,posy,grid[posz][posy][posx]);
}

void EvidenceGrid<unsigned char>::fill(unsigned char val)
{
//	unsigned char counter=0;
	for (int zz=0; zz<=zsz-1; ++zz)
	{
		for (int yy=0; yy<=ysz-1; ++yy)
		{
			for (int xx=0; xx<=xsz-1; ++xx)
			{
				grid[zz][yy][xx]=val;
			//	grid[zz][yy][xx]=counter++;
			}
		}
	}
}

void EvidenceGrid<unsigned char>::rotate_into(EvidenceGrid* dest_grid,
													  int layer,int angle)
{
	double rad_angle=(M_PI/180)*angle;
	double cos_theta=std::cos(rad_angle);
	double sin_theta=std::sin(rad_angle);

	for (int yy=0; yy<=ysz-1; ++yy)
	{
		for (int xx=0; xx<=xsz-1; ++xx)
		{
			dest_grid->grid[layer]
						   [static_cast<int>(xx*sin_theta+yy*cos_theta)]
						   [static_cast<int>(xx*cos_theta-yy*sin_theta)]
						   =grid[layer][yy][xx];
		}
	}
}

EvidenceGrid<unsigned char>::~EvidenceGrid()
{
	deallocate();
}
