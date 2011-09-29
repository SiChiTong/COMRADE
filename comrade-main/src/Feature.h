#ifndef FEATURE_H
#define FEATURE_H

#include <iostream>
#include <string>
#include <vector>
#include "SystemDefs.h"
#include "IoRedir.h"

// NOTE: Features are hardcoded for now. More will be added.
// The current ones may be modified.

#ifdef USING_LINUX
#include "libfg_cpp.h"
#include "SerialRobotInterface.hh"
#endif

namespace Comrade {
    namespace Osiris {
    
class Feature
{
protected:  std::string identifier;
            bool active;
			
public:     Feature();

            virtual bool is_present()=0;

            void set_identifier(std::string id);
            std::string get_identifier();

            virtual ~Feature();
};

class USB_CameraFeature:public Feature
{
private:

public:
			#ifdef USING_LINUX
			FRAMEGRABBER* grabber_ptr;
			FRAME* temp_frame;
			#endif

			USB_CameraFeature();
			bool is_present();
            		
			~USB_CameraFeature();
};

// Since the serial link ptr is present in the serial features,
// they can all use the same COM port (which'll be the case)

class ArmFeature:public Feature
{
private:

public:     
			#ifdef USING_LINUX
			Comrade::Corelib::RobotInterface* interface;
			#endif
			
			ArmFeature();
            bool is_present();
            ~ArmFeature();
};

// NOTE: Deprecated in v2.00, superceded by Comrade::Corelib::MotorController...

/*class ActuatorFeature:public Feature
{
private:

public:     
			#ifdef USING_LINUX
			Comrade::Corelib::RobotInterface* interface;
			#endif
			
            ActuatorFeature();
			bool is_present();
            ~ActuatorFeature();
};*/

}
}

#endif
