#ifndef ABSTRACT_HARDWARE_H
#define ABSTRACT_HARDWARE_H

#include <iostream>
#include "SystemDefs.h"
#include "IoRedir.h"
#include "Dependency.h"

#ifdef USING_LINUX
#include "SerialRobotInterface.hh"
//#include "StereovisionMotor.h"
#endif

// NOTE: The USB libfg interface has been merged inside RGB_BufferManager.
//       We do not need to include any special headers here.

namespace Comrade {
    namespace Osiris {

class AbstractHardware
{
protected:  Dependency depends;

public:	    USB_CameraFeature cam;
            //ActuatorFeature act; // superceded by MotorController structure
            ArmFeature arm;
	    #ifdef USING_LINUX
	    MotorController* motor_controller;
	    StereovisionMotor stereovision_motor;
	    #endif

		FeatureList obj_detect;
           	FeatureList obj_retrieve;
           	FeatureList line_follow;
		FeatureList stereovision;

			#ifdef USING_LINUX
			Comrade::Corelib::RobotInterface* interface; // For sensor data acquisition
			#endif
			

			int sonar_x,sonar_y;
			int odometry_x,odometry_y;
			
public:     AbstractHardware();

            bool check(std::string ability);

			~AbstractHardware();
};

}
}

#endif
