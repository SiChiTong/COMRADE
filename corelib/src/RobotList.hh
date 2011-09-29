#ifndef COMRADE_CORELIB_ROBOTLIST
#define COMRADE_CORELIB_ROBOTLIST

#include <map>
#include "FrameCache.hh"
#include "Singleton.hh"

namespace Comrade
{
	
namespace Corelib
{

class RobotDetails
{
	public:
		RobotDetails();
		~RobotDetails();
		void add_frame(Frame* frame);
		void set_nextframeid(guint32 id);
		void process_pending_frames();
	private:
		guint32 nextframeid;
		FrameCache inframecache;
};

class RobotList: public Singleton<RobotList>
{
	public:
		RobotList();
		~RobotList();
		RobotDetails* get_details(guint32 robot_id);
	private:
		std::map<guint32, RobotDetails*> robotlist;
};

} // namespace Corelib

} // namespace Comrade

#endif
