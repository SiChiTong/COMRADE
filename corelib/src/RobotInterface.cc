#include "RobotInterface.hh"

#include <glibmm/random.h>

using namespace Comrade::Corelib;

RobotInterface::RobotInterface()
{
	Glib::Rand rand;
	robot_id=rand.get_int();
	log=Logger::get_singleton().get_category_ptr("Robot");
	*log << logINFO << "Generated robot id " << robot_id << logENDL;
}

RobotInterface::~RobotInterface()
{
}

guint32 RobotInterface::get_id()
{
	return robot_id;
}
