#include "Framer.hh"
#include "Configurator.hh"
#include "RobotInterface.hh"
#include "RobotList.hh"

#include <sys/time.h>
#include <sigc++/sigc++.h>

using namespace std;
using namespace Comrade::Corelib;

Framer::Framer()
{
	log=Logger::get_singleton().get_category_ptr("Framer");
	Configurator &c=Configurator::get_singleton();
	
	maxframesize=c.get_network_txpacketsize();
	multicaster=new Multicaster;
	multicaster->on_data_receive.connect(sigc::mem_fun(*this,&Framer::receiver));
	outframecache= new FrameCache;
	seqno=0;
}

Framer::~Framer()
{
	delete outframecache;
	delete multicaster;
}

void Framer::transmit(unsigned char* data, int datasize)
{
	int ptr=0;
	int ds;
	do
	{
		if (datasize<=maxframesize) ds=datasize;
			else ds=maxframesize;
		Frame* txframe=new DataFrame(data+ptr,ds);
		txframe->transmit();
		outframecache->add_frame(txframe);
		ptr+=ds;
		datasize-=ds;
	}
	while (datasize);
}


void Framer::receiver(unsigned char *data, int datasize)
{
	*log << logINFO << "Received " << datasize << " bytes" << logENDL;
	Frame *rxframe=new Frame(data,datasize);
	*log << logINFO << "created frame" << logENDL;
	Frame *newrxframe=rxframe->decode(); //now it contains the derived class
	*log << logDEBUG << "Frame of type " << newrxframe->get_frame_type() << " received" << logENDL;
	if (newrxframe->get_frame_type()==FRAMETYPE_HELLO)
	{
		newrxframe->process();
	}
	else
	{
		RobotDetails *rd=RobotList::get_singleton().get_details(newrxframe->getfromid());
		if (rd)
		{
			rd->add_frame(newrxframe);
			rd->process_pending_frames();
		}
		else
		{
			//wtf.. there's a robot we don't know about
			*log << logERROR << "Frame received from unknown robot" << logENDL;
		}
	}
	rxframe->dont_destroy_data();
	delete rxframe;
}

guint64 Framer::get_timestamp()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return (guint64(tv.tv_sec)*1000000)+guint64(tv.tv_usec);
}

guint32 Framer::get_seqno()
{
	return seqno++;
}
