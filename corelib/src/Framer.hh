#ifndef COMRADE_CORELIB_FRAMER
#define COMRADE_CORELIB_FRAMER

#include "Multicaster.hh"
#include "Logger.hh"
#include "Singleton.hh"
#include "FrameCache.hh"
#include <netinet/in.h>
#include <sigc++/sigc++.h>

namespace Comrade
{

namespace Corelib
{
	
class Framer: public Singleton<Framer>, public sigc::trackable
{
	public:
		Framer();
		~Framer();
		void transmit(unsigned char* data, int datasize);
		sigc::signal<void, unsigned char*, int> on_frame_receive;
		guint64 get_timestamp();
		guint32 get_seqno();

	private:
		Multicaster *multicaster;
		log4cpp::Category *log;
		FrameCache *outframecache;
		guint32 seqno; // Sequence number of next frame to be sent
		int maxframesize;
	
		void receiver(unsigned char *data, int datasize);
	
};
	
} //namespace Corelib

} //namespace Comrade

#endif
