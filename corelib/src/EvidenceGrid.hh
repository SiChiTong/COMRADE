#ifndef EVIDENCE_GRID_HH
#define EVIDENCE_GRID_HH

//#include "newmath.h"
#include "Constants.hh"
#include "SectorDraw.hh"
#include "CIRC.hh"
#include <cmath>
#include <iostream>
#include <sigc++/sigc++.h>
#include <utility>

namespace Comrade
{
namespace Osiris
{
	class LocalisationEngine;
}
}

namespace Comrade {
	
namespace Corelib {
	
template <typename Some>
class EvidenceGrid
{
protected:	Some*** grid;
			int zsz,ysz,xsz;

			void allocate(int z,int y,int x);
			void deallocate();
			
public:		friend class Comrade::Osiris::LocalisationEngine;
	
			EvidenceGrid(int z,int y,int x);

			int return_z()
			{
				return zsz;
			}
			
			int return_y()
			{
				return ysz;
			}
			
			int return_x()
			{
				return xsz;
			}
			
			Some& at(int z,int y,int x)
			{
				return grid[z][y][x];
			}
			void update(std::pair<int,int> point, int posz, int radius, int reading,
						double angle, bool is_obstacle, std::pair<int,int> offset);
//			void update_for_vfh(std::pair<int,int> point,int posz,int radius, 
//						int reading, double angle, bool is_obstacle,
//						std::pair<int,int> offset);

			void fill(Some val);

			void rotate_into(EvidenceGrid* dest_grid,int layer,int angle);
			
			~EvidenceGrid();
			
			sigc::signal<void,int,int,int> on_update;
};

template<> class EvidenceGrid<unsigned char>
{
protected:	unsigned char*** grid;
		int zsz,ysz,xsz;
		int sonar_age;

		void allocate(int z,int y,int x);
		void deallocate();
			
public:		friend class LocalisationEngine;
	
			EvidenceGrid(int z,int y,int x);

			int return_z()
			{
				return zsz;
			}
			
			int return_y()
			{
				return ysz;
			}
			
			int return_x()
			{
				return xsz;
			}
			
			unsigned char& at(int z,int y,int x)
			{
				return grid[z][y][x];
			}
			void update(std::pair<int,int> point, int posz, int radius, int reading,
						double angle, bool is_obstacle, std::pair<int,int> offset);
			void update_vfh(std::pair<int,int> point,int posz,int radius, 
						int reading, double angle, bool is_obstacle,
						std::pair<int,int> offset);
			void update_from_sonar(map_update& mu, SectorDraw &sectordraw);
			unsigned char get_probability(int z, int y, int x);
			void set_sonar_age(int age);

			void fill(unsigned char val);

			void rotate_into(EvidenceGrid* dest_grid,int layer,int angle);
			
			~EvidenceGrid();
			sigc::signal<void,int,int,int> on_update;
};
	
template <typename Some>
void Comrade::Corelib::EvidenceGrid<Some>::allocate(int z,int y,int x)
{
	zsz=z;
	ysz=y;
	xsz=x;
		
	grid=new Some**[zsz];
		
	for (int i=0; i<=zsz-1; ++i)
	{
		grid[i]=new Some*[ysz];
			
		for (int j=0; j<=ysz-1;++j)
		{
			grid[i][j]=new Some[xsz];
		}
	}
}

template <typename Some>
void Comrade::Corelib::EvidenceGrid<Some>::deallocate()
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

template <typename Some>
Comrade::Corelib::EvidenceGrid<Some>::EvidenceGrid(int z,int y,int x)
{
		allocate(z,y,x);
}

template <typename Some>
void Comrade::Corelib::EvidenceGrid<Some>::update
(std::pair<int,int> point,int posz,
int radius, int reading, double angle, bool is_obstacle,std::pair<int,int> offset)
{}

template <typename Some>
void Comrade::Corelib::EvidenceGrid<Some>::fill(Some val)
{
	for (int zz=0; zz<=zsz-1; ++zz)
	{
		for (int yy=0; yy<=ysz-1; ++yy)
		{
			for (int xx=0; xx<=xsz-1; ++xx)
			{
				grid[zz][yy][xx]=val;
			}
		}
	}
}

template <typename Some>
void Comrade::Corelib::EvidenceGrid<Some>::rotate_into(EvidenceGrid<Some>* dest_grid,
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

template <typename Some>
Comrade::Corelib::EvidenceGrid<Some>::~EvidenceGrid()
{
	deallocate();
}

}
}

#endif
