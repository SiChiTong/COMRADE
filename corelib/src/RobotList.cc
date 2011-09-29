#include "RobotList.hh"

using namespace Comrade::Corelib;

RobotDetails::RobotDetails()
{
}

RobotDetails::~RobotDetails()
{
}

void RobotDetails::process_pending_frames()
{
	nextframeid=inframecache.process_from(nextframeid);
}

void RobotDetails::set_nextframeid(guint32 id)
{
	nextframeid=id;
}

void RobotDetails::add_frame(Frame* frame)
{
	inframecache.add_frame(frame);
}

RobotList::RobotList()
{
}

RobotList::~RobotList()
{
}


RobotDetails* RobotList::get_details(guint32 robot_id)
{
	std::map<guint32, RobotDetails*>::iterator iter=robotlist.find(robot_id);
	if (iter==robotlist.end()) return NULL;
		else return (*iter).second;
}
