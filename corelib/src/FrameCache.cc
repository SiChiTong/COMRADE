#include "FrameCache.hh"
#include <map>
#include <set>

using namespace std;
using namespace Comrade::Corelib;


FrameCache::FrameCache()
{
	
}

FrameCache::~FrameCache()
{	
}

void FrameCache::add_frame(Frame* frame)
{
	cache.insert(make_pair(frame->get_seqno(), frame));
}

void FrameCache::ack_frame(guint32 sequence_no, guint32 node_id)
{
	Frame* frame = cache[sequence_no];
	if(frame->ack_node(node_id))
	{
		delete frame;
		cache.erase(sequence_no);
	}
}

guint32 FrameCache::process_from(guint32 id)
{
	std::map<guint32, Frame*>::iterator iter;
	while(true)
	{
		iter=cache.find(id);
		if (iter==cache.end())
		{
			break;
		}
		Frame *f=iter->second;
		f->process();
		f->dont_destroy_data();
		delete f;
		cache.erase(iter);
		id++;
	}
	return id;
}
