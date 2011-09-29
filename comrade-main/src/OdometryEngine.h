#ifndef ODOMETRY_ENGINE_H
#define ODOMETRY_ENGINE_H

#include "MouseOdometer.hh"

namespace Comrade
{
	
namespace ComradeRuntime
{

class OdometryEngine
{
	public:
		OdometryEngine();
		~OdometryEngine();
		double get_x() const;
		double get_y() const;
		double get_orientation() const;
		void mouse_data_receive(int left, int right);
	private:
		double x,y;
		double orientation;
		Comrade::Corelib::MouseOdometer mouse_odometer;
};

}
}

#endif
