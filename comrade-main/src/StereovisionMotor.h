#ifndef STEREOVISION_MOTOR_H
#define STEREOVISION_MOTOR_H

#include "Feature.h"

namespace Comrade {
	namespace Osiris {
		
class StereovisionMotor: public Feature
{
public:		
		#ifdef USING_LINUX
		Comrade::Corelib::RobotInterface* interface;
		#endif

public:		StereovisionMotor();
		bool is_present();
		~StereovisionMotor();
};

}
}

#endif
