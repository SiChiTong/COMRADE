#ifndef COMRADE_CORELIB_FRAMECACHE
#define COMRADE_CORELIB_FRAMECACHE

#include <netinet/in.h>
#include <set>
#include <map>
#include "Frame.hh"

namespace Comrade
{

namespace Corelib
{

/* Some explanation required here:
	
Each Frame has to be acknowledged by all the known nodes to be "fully
acknowledged". So each frame has a list of all the known recipient node ids in
ack_states, from which we keep removing node ids using ack_node(). If ack_node()
returns true, it means that the frame is fully acknowledged and can be removed
from the FrameCache.
	
*/	

class FrameCache
{
	public:
		FrameCache();
		~FrameCache();
		
		void add_frame(Frame* frame);
		void ack_frame(guint32 sequence_no, guint32 node_id);
		guint32 process_from(guint32 id);
	private:
		std::map<guint32, Frame*> cache; // guint32 holds the sequence number
};

} // namespace Corelib

} // namespace Comrade

#endif
