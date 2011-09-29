#include "Frame.hh"
#include "RobotInterface.hh"
#include "Framer.hh"
#include <iostream>

using namespace Comrade::Corelib;
using namespace std;

Frame::Frame()
{
	//we need to initialize list of nodes here.
	txbuffer=NULL;
	txbuffersize=0;
}

Frame::Frame(unsigned char *data, int datasize)
{
	cout << "creating frame" << endl;
	txbuffer=new unsigned char[datasize];
	memcpy((void*)txbuffer, (void *)data, datasize);
	txbuffersize=datasize;
}

Frame::~Frame()
{
	if (txbuffer && txbuffersize)
		delete[] txbuffer;
}

Frame::Frame(Frame *f)
{
	frame_type=f->frame_type;
	from_id=f->from_id;
	timestamp=f->timestamp;
	sequence_no=f->sequence_no;
	no_of_acks=f->no_of_acks;
	txbuffer=f->txbuffer;
	txbuffersize=f->txbuffersize;
}

Frame* Frame::decode()
{
	cout << "decoding frame" << endl;
	sFrame sf;
	memcpy((void *)&sf, (void *)txbuffer, sizeof (sFrame));
	frame_type=FrameType(sf.frame_type);
	from_id=sf.from_id;
	timestamp=sf.time_stamp;
	sequence_no=sf.sequence_no;
	no_of_acks=sf.no_of_acks;
	Frame *ret;
	cout << "frame type = " << frame_type << endl;
	switch (frame_type)
	{
		case FRAMETYPE_DATA:
			ret=new DataFrame(this);
			break;
		case FRAMETYPE_ACK:
			ret=new AckFrame(this);
			break;
		case FRAMETYPE_NACK:
			ret=new NackFrame(this);
			break;
		case FRAMETYPE_HELLO:
			ret=new HelloFrame(this);
			break;
		case FRAMETYPE_WELCOME:
			ret=new WelcomeFrame(this);
			break;
		default:
			cout << "Invalid frame type" << endl;
			//very very bad
			break;
	}
	return ret;
}

void Frame::process()
{
}

void Frame::prepare_txbuffer()
{
}

bool Frame::ack_node(guint32 node_id)
{
	ack_states.erase(node_id);
	if(ack_states.empty()) return true;
		else return false;
}

void Frame::dont_destroy_data()
{
	txbuffersize=0;
}

guint32 Frame::get_seqno()
{
	return sequence_no;
}

unsigned char* Frame::get_txbuffer()
{
	return txbuffer;
}

sFrame* Frame::fill_frame_header()
{
	sFrame *temp;
	temp=new sFrame;
	temp->frame_type=frame_type;
	temp->from_id=RobotInterface::get_singleton().get_id();
	timestamp=Framer::get_singleton().get_timestamp();
	temp->time_stamp=timestamp;
	temp->sequence_no=Framer::get_singleton().get_seqno();
	if (frame_type==FRAMETYPE_ACK)
		temp->no_of_acks=1;
	else temp->no_of_acks=0;
	return temp;
}

void Frame::transmit()
{
	prepare_txbuffer();
	Multicaster::get_singleton().transmit(txbuffer,txbuffersize);
}

guint32 Frame::getfromid()
{
	return from_id;
}

FrameType Frame::get_frame_type()
{
	return frame_type;
}

DataFrame::DataFrame(unsigned char *data, int datasize)
{
	this->data=data;
	this->data_size=datasize;
	this->frame_type=FRAMETYPE_DATA;
}

DataFrame::DataFrame(Frame *f):
	Frame(f)
{
	unsigned char* buf=f->get_txbuffer();
	memcpy((void*)&data_size,(void*)(buf+sizeof(sFrame)),2);
	data=buf+sizeof(sFrame)+2;
}

DataFrame::~DataFrame()
{
}

void DataFrame::prepare_txbuffer()
{
	sFrame *txframe=fill_frame_header();
	txbuffersize=sizeof(sFrame)+2+data_size;
	txbuffer=new unsigned char[txbuffersize];
	memcpy((void*)txbuffer,(void*)txframe,sizeof(sFrame));
	memcpy((void*)(txbuffer+sizeof(sFrame)),(void*)&data_size,2);
	memcpy((void*)(txbuffer+sizeof(sFrame)+2),(void*)data,data_size);
}

void DataFrame::process()
{
}

AckFrame::AckFrame(guint32 node_id, guint64 timestamp, guint32 seqno)
{
	this->node_id=node_id;
	this->timestamp=timestamp;
	this->seqno=seqno;
	this->frame_type=FRAMETYPE_ACK;
}

AckFrame::~AckFrame()
{
}

AckFrame::AckFrame(Frame *f):
	Frame(f)
{
	unsigned char* buf=f->get_txbuffer();
	sAcknowledgement sA;
	memcpy((void*)&sA,(void*)(buf+sizeof(sFrame)),sizeof(sAcknowledgement));
	node_id=sA.to_id;
	seqno=sA.sequence_no;
}

void AckFrame::prepare_txbuffer()
{
	sFrame *txframe=fill_frame_header();
	txbuffersize=sizeof(sFrame)+sizeof(sAcknowledgement);
	txbuffer=new unsigned char[txbuffersize];
	sAcknowledgement sA;
	sA.to_id=node_id;
	sA.time_stamp=timestamp;
	sA.sequence_no=seqno;
	memcpy((void*)txbuffer,(void*)txframe,sizeof(sFrame));
	memcpy((void*)(txbuffer+sizeof(sFrame)),(void*)&sA,sizeof(sAcknowledgement));
}

void AckFrame::process()
{
}

NackFrame::NackFrame(guint32 node_id, guint64 timestamp, guint32 seqno)
{
	this->frame_type=FRAMETYPE_NACK;
}

NackFrame::NackFrame(Frame* f)
{
}

NackFrame::~NackFrame()
{
}

void NackFrame::prepare_txbuffer()
{
}

void NackFrame::process()
{
}

HelloFrame::HelloFrame()
{
	this->frame_type=FRAMETYPE_HELLO;
}

HelloFrame::HelloFrame(Frame* f)
{
}

HelloFrame::~HelloFrame()
{
}

void HelloFrame::prepare_txbuffer()
{
}

void HelloFrame::process()
{
}

WelcomeFrame::WelcomeFrame()
{
	this->frame_type=FRAMETYPE_WELCOME;
}

WelcomeFrame::WelcomeFrame(Frame* f)
{
}

WelcomeFrame::~WelcomeFrame()
{
}

void WelcomeFrame::prepare_txbuffer()
{
}

void WelcomeFrame::process()
{
}
