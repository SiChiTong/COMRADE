#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "RobotInterface.hh"
#include "Feature.h"

namespace Comrade
{
	
namespace Osiris
{

class MotorController: public Feature
{
	public:
		MotorController(Comrade::Corelib::RobotInterface *ri);
		~MotorController();
		void set_target_speeds(int left, int right);
		bool timeout_handler();
		bool is_present();
		void set_target_direction(double dest_angle, double r);

	private:
		int calc_new_speed(int old, int target);
		int curleft, curright;
		int targetright, targetleft;
		Comrade::Corelib::RobotInterface *ri;
};
	
}
}

#endif
