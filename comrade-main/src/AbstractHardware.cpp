#include "../config.h"
#include "AbstractHardware.h"

extern Comrade::IO::IO_Redirector stream;

Comrade::Osiris::AbstractHardware::AbstractHardware()
{
#ifdef USING_LINUX
	// Using a single serial interface to control all sensors/actuators...
	interface=new Comrade::Corelib::SerialRobotInterface();
	arm.interface=interface;
	//stereovision_motor.interface=interface;
	motor_controller=new MotorController(interface);
#endif
		
	obj_detect.add_feature(&cam);
	obj_detect.add_feature(motor_controller);
	
	obj_retrieve.add_feature(motor_controller);
	obj_retrieve.add_feature(&arm);
	
	line_follow.add_feature(&cam);
	line_follow.add_feature(motor_controller);
	
	stereovision.add_feature(&cam);
	//stereovision.add_feature(&stereovision_motor);

	depends.dep_map["OBJ_DET"]=&obj_detect;
	depends.dep_map["OBJ_RET"]=&obj_retrieve;
	depends.dep_map["LINE_FOLL"]=&line_follow;
	//depends.dep_map["STEREO_VISION"]=&stereovision;

	stream.write("AbstractHardware Runtime for Windows/Linux loaded...\n");
}

bool Comrade::Osiris::AbstractHardware::check(std::string ability)
{
 	if (depends.dep_map[ability]->is_complete()==true)
	{
		stream.write("Can perform ");
		stream.write(ability);
		stream.write("...\n");
		return true;
	}
	
	else
	{
		stream.write("Cannot perform ");
		stream.write(ability);
		stream.write("...\n");
		return false;
	}
}


Comrade::Osiris::AbstractHardware::~AbstractHardware()
{
	#ifdef USING_LINUX
	delete motor_controller;
	delete interface;
	#endif
	
}
