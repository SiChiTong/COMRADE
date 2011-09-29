#ifndef COMRADE_CORELIB_FRAME
#define COMRADE_CORELIB_FRAME

#include <set>
#include "glib/gtypes.h"

namespace Comrade
{
	
namespace Corelib
{

enum FrameType {FRAMETYPE_DATA, FRAMETYPE_ACK, FRAMETYPE_NACK, FRAMETYPE_HELLO, FRAMETYPE_WELCOME};
	
struct sFrame
{
	unsigned char frame_type;
	guint32 from_id;
	guint64 time_stamp;
	guint32 sequence_no;
	unsigned char no_of_acks;
};

struct sAcknowledgement
{
	guint32 to_id;
	guint64 time_stamp;
	guint32 sequence_no;
};
	
class Frame
{
	public:
		Frame();
		Frame(unsigned char *data, int datasize);
		Frame(Frame *f);
		virtual ~Frame();
		bool ack_node(guint32 node_id); // returns whether the frame is fully acknowledged.
		void transmit();
		virtual void process();
		guint32 getfromid();
		Frame* decode();
		guint32 get_seqno();
		FrameType get_frame_type();
		void dont_destroy_data();
		unsigned char* get_txbuffer();
	
	protected:
		FrameType frame_type;
		guint32 from_id;
		unsigned char* txbuffer;
		unsigned int txbuffersize;
		unsigned char no_of_acks;
		guint64 timestamp; // holds the timestamp when the packet was sent
		guint32 sequence_no;
		std::set<guint32> ack_states; // the guint32 holds the node id
		virtual void prepare_txbuffer();
		sFrame* fill_frame_header();
};

class DataFrame:public Frame
{
	public:
		DataFrame(unsigned char *data, int datasize);
		DataFrame(Frame *f);
		~DataFrame();
	private:
		void prepare_txbuffer();
		void process();
		unsigned char* data;
		guint16 data_size;
};

class AckFrame:public Frame
{
	public:
		AckFrame(guint32 node_id, guint64 timestamp, guint32 seqno);
		AckFrame(Frame *f);
		~AckFrame();
	private:
		guint32 node_id;
		guint32 seqno;
		void prepare_txbuffer();
		void process();
};

class NackFrame:public Frame
{
	public:
		NackFrame(guint32 node_id, guint64 timestamp, guint32 seqno);
		NackFrame(Frame *f);
		~NackFrame();
	private:
		guint32 node_id;
		guint32 seqno;
		void prepare_txbuffer();
		void process();
};

class HelloFrame:public Frame
{
	public:
		HelloFrame();
		HelloFrame(Frame *f);
		~HelloFrame();
	private:
		void prepare_txbuffer();
		void process();
};

class WelcomeFrame:public Frame
{
	public:
		WelcomeFrame();
		WelcomeFrame(Frame *f);
		~WelcomeFrame();
	private:
		void prepare_txbuffer();
		void process();
};

} // namespace Corelib

} // namespace Comrade

#endif
