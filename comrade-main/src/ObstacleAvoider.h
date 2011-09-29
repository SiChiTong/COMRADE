#ifndef OBSTACLE_AVOIDER_H
#define OBSTACLE_AVOIDER_H

#include "EvidenceGrid.hh"
#include "SectorDraw.hh"
#include "OdometryEngine.h"
#include <utility>

namespace Comrade
{
	
namespace ComradeRuntime
{

struct oalookuptable
{
	double beta,gamma,radiusfactor;
	bool in_circle;
	int startsector,endsector;
};

#define OBSTACLE_AVOIDER_SEARCH_RADIUS 100

class ObstacleAvoider
{
	public:
		ObstacleAvoider(OdometryEngine *oe, Comrade::Corelib::EvidenceGrid<unsigned char> *eg);
		~ObstacleAvoider();
		std::pair<double,double> get_speeds();
		void check_point(std::pair<int,int> point,int posz, int radius,
			int reading, double angle, bool is_obstacle,
			std::pair<int,int> offset);
	
	private:
		OdometryEngine* odometry_engine;
		Comrade::Corelib::SectorDraw sectordraw;
		Comrade::Corelib::EvidenceGrid<unsigned char> *evidence_grid;
		double *histogram;
		bool *binhistogram;
		bool *maskedhistogram;
		double *density;
		int noofsectors;
		int cursector;
		int lastvel;
		oalookuptable oalt[OBSTACLE_AVOIDER_SEARCH_RADIUS*2][OBSTACLE_AVOIDER_SEARCH_RADIUS*2];
};
	
}
}

#endif
