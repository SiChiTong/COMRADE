#include "RobotInterface.hh"
#include "SerialRobotInterface.hh"
#include "Configurator.hh"
#include "SerialInstructions.hh"
#include <iostream>
#include "glibmm/main.h"

using namespace Comrade::Corelib;
using namespace std;


SerialRobotInterface::SerialRobotInterface()
{
	serialhandler=new SerialHandler;
	log=Logger::get_singleton().get_category_ptr("SerialFramer");
	Configurator &c=Configurator::get_singleton();
	
	serialhandler->on_serial_data_receive.connect(sigc::mem_fun(*this,&SerialRobotInterface::receiver));
	maxframesize=c.get_serial_max_framesize();
	
	reload_config();

	framebuffer=new unsigned char[maxframesize];
	framebufferlen=0;
	nextframeid=-1;
	timeout_active=false;
	compass_mapping=true;
}

SerialRobotInterface::~SerialRobotInterface()
{
	delete serialhandler;
}

void SerialRobotInterface::enable_compass_mapping(bool enable)
{
	compass_mapping=enable;
}

void SerialRobotInterface::reload_config()
{
	Configurator &c=Configurator::get_singleton();

	leftinvert=c.get_motorspeeds_leftinvert();
	rightinvert=c.get_motorspeeds_rightinvert();
	
	lfmax=c.get_motorspeeds_leftforward();
	lbmax=c.get_motorspeeds_leftbackward();
	rfmax=c.get_motorspeeds_rightforward();
	rbmax=c.get_motorspeeds_rightbackward();
	
	lfmin=c.get_motorspeeds_minleftforward();
	lbmin=c.get_motorspeeds_minleftbackward();
	rfmin=c.get_motorspeeds_minrightforward();
	rbmin=c.get_motorspeeds_minrightbackward();

	compassmax=c.get_compass_maxvalue();
	compassmin=c.get_compass_minvalue();
}

void SerialRobotInterface::receiver(unsigned char* data, int data_size)
{
	for (int i=0;i<data_size;i++)
	{
		process_char(data[i]);

	}
}

bool SerialRobotInterface::transmit_timeout()
{
	*log << logDEBUG << "Entered 100ms timeout" << logENDL;
	//if (timeout_active==false) return false;
	return send_next_packet();
}

bool SerialRobotInterface::send_next_packet()
{
	*log << logDEBUG << "send_next_packet() called" << logENDL;
	if (!outframequeue.empty())
	{
		*log << logDEBUG << "sending next packet" << logENDL;
		SerialFrame *f=outframequeue.front();
		f->transmit(serialhandler);
		*log << logDEBUG << "sent next packet" << logENDL;
		return true;
	}
	else
	{
		*log << logDEBUG << "queue empty" << logENDL;
		timeout_active=false;
		return false;
	}
}

void SerialRobotInterface::process_char(unsigned char c)
{
	static int recvd=0;
	static SerialFrameState state=FRAME_NOT_STARTED;
	
	try
	{
		if (state==FRAME_NOT_STARTED)
		{
			if (c==START_FLAG)
			{
				framebufferlen=0;
				state=FRAME_STARTED;
			}
			else throw(SerialFrameError("Character other than start flag encountered when not in frame"));
		}
		else
		{
			if (c==START_FLAG)
			{
				framebufferlen=0;
				throw (SerialFrameError("Unexpected start flag encountered"));
			}
			else if (c==STOP_FLAG)
			{
				state=FRAME_NOT_STARTED;
				if (framebufferlen==0)
				{
					//it's an ACK
					//serialhandler->missed_acks--;
					recvd++;
					*log << logDEBUG << "ACK recvd" << logENDL;
					*log << logDEBUG << "Packets txed: "<<serialhandler->missed_acks << " recvd: " << recvd << logENDL;
					if (!outframequeue.empty()) outframequeue.pop_front();
					timeout_conn.disconnect();
					timeout_active=false;
					if (send_next_packet())
					{
						reattach_timeout();
					}					
					
				}
				else
				{
					SerialFrame *sf=new SerialFrame(framebuffer,framebufferlen);
					sf->process(this);
					delete sf;
				}
//				process_frame();
			}
			else
			{
				if (framebufferlen==maxframesize)
				{
					framebufferlen=0;
					state=FRAME_NOT_STARTED;
					throw (SerialFrameError("Buffer length overflow"));
				}
				framebuffer[framebufferlen++]=c;
			}
		}
	}
	catch(SerialFrameError &sfe)
	{
		sfe.log_error();
	}
}

// Do this only if we are actually using the frame ids. Doesn't look like it's needed.
/*void SerialRobotInterface::process_frame()
{
	SerialFrame *sf=new SerialFrame(framebuffer,framebufferlen);
	framebufferlen=0;
	int fid=sf->get_frame_id();
	if (nextframeid==-1 || fid==nextframeid)
	{
		nextframeid=(fid+1)%20;
		sf->process();
		std::map<int, SerialFrame*>::iterator iter;
		bool finished=false;
		do
		{
			iter=inframecache.find(nextframeid);
			if (iter!=inframecache.end())
			{
				iter->second->process();
				nextframeid=(nextframeid+1)%20;
			}
			else finished=true;
		}
		while (!finished);
	}
	else
	{ // Out of order frame encountered :-(
		*log << logERROR << "Out of order frame encountered." << logENDL;
		if (inframecache.find(fid)==inframecache.end())
			inframecache.insert(make_pair(sf->get_frame_id(), sf));
		else
			inframecache[fid]=sf;
		//TODO: Send nack here
	}
}*/

void SerialRobotInterface::reattach_timeout()
{
	if (timeout_conn) timeout_conn.disconnect();
	timeout_conn=Glib::signal_timeout().connect(sigc::mem_fun(this,&SerialRobotInterface::transmit_timeout),100);
}

void SerialRobotInterface::queue_frame(unsigned char instruction, unsigned char* payload, unsigned char payload_size)
{
	static int queued=0;
	queued++;
	*log << logDEBUG << "Queued frames: " << queued << logENDL;
	SerialFrame *sf =new SerialFrame(instruction,payload,payload_size);
	if (outframequeue.empty())
	{
		outframequeue.push_back(sf);
		send_next_packet();
		reattach_timeout();
	}
	else
	{
		outframequeue.push_back(sf);
		//send_next_packet(); // Should not be here
	}
	*log << logDEBUG << "Queue size: " << outframequeue.size() << logENDL;
}

double SerialRobotInterface::map_values(double from_start, double from_end, double to_start, double to_end, double value)
{
	return (((value-from_start)/(from_end-from_start))*(to_end-to_start))+to_start;
}

void SerialRobotInterface::mapped_steer(int left, int right)
{
	int dleft,dright;
	if (left<0)
		dleft=int(map_values(0,-127.0,double(lbmin),double(lbmax),double(left)));
	else if (left>0)
		dleft=int(map_values(0,127.0,double(lfmin),double(lfmax),double(left)));
	else if (left==0)
		dleft=0;
	if (right<0)
		dright=int(map_values(0,-127.0,double(rbmin),double(rbmax),double(right)));
	else if (right>0)
		dright=int(map_values(0,127.0,double(rfmin),double(rfmax),double(right)));
	else if (right==0)
		dright=0;
	steer(dleft,dright);
	/*cout << "Speeds " << left << " and " << right << " mapped to "
		<< dleft << " and " << dright << endl;*/
}

void SerialRobotInterface::steer(int left, int right)
{
	unsigned char pl[2];
	if (leftinvert) left=-left;
	if (rightinvert) right=-right;
	if (left>127) left=127;
	if (left<-127) left=-127;
	if (right>127) right=127;
	if (right<-127) right=-127;
	if (left>0) pl[0]=left;
		else pl[0]=128-left;
	if (right>0) pl[1]=right;
		else pl[1]=128-right;
	if (pl[0]>=254) pl[0]=253;
	if (pl[1]>=254) pl[1]=253;
	queue_frame(STEER,pl,2);
}

void SerialRobotInterface::start()
{
	queue_frame(START_EVERYTHING,NULL,0);
}

void SerialRobotInterface::stop()
{
	queue_frame(STOP_EVERYTHING,NULL,0);
}

void SerialRobotInterface::camera_translate(int angles)
{
	unsigned char pl[2];
	if (angles>127) angles=127;
	if (angles<-127) angles=-127;
	if (angles>0) pl[0]=angles;
		else pl[0]=128-angles;
	if (pl[0]>=254) pl[0]=253;
	pl[1]=0;
	queue_frame(CAMERA_TRANSLATE,pl,2);
}

void SerialRobotInterface::camera_translate_set_steps_per_angle(int steps)
{
	unsigned char pl[2];
	if (steps<0) steps=0;
	if (steps>253) steps=253;
	pl[0]=steps;
	pl[1]=1;
	queue_frame(CAMERA_TRANSLATE,pl,2);
}

void SerialRobotInterface::camera_rotate(int angles)
{
	unsigned char pl[2];
	if (angles>127) angles=127;
	if (angles<-127) angles=-127;
	if (angles>0) pl[0]=angles;
		else pl[0]=128-angles;
	if (pl[0]>=254) pl[0]=253;
	pl[1]=0;
	queue_frame(CAMERA_ROTATE,pl,2);
}

void SerialRobotInterface::camera_rotate_set_steps_per_angle(int steps)
{
	unsigned char pl[2];
	if (steps<0) steps=0;
	if (steps>253) steps=253;
	pl[0]=steps;
	pl[1]=1;
	queue_frame(CAMERA_ROTATE,pl,2);
}

void SerialRobotInterface::sonar_calibrate_start(int angle)
{
	unsigned char pl[1];
	pl[0]=angle;
	queue_frame(SONAR_START_CALIBRATE,pl,1);
}

void SerialRobotInterface::sonar_calibrate_stop()
{
	queue_frame(SONAR_STOP_CALIBRATE,NULL,0);
}

void SerialRobotInterface::initialize()
{
}

void SerialRobotInterface::move_arm(int armno, double angle)
{
}

SerialFrameError::SerialFrameError(string error_string)
{
	log=Logger::get_singleton().get_category_ptr("SerialFramer");
	this->error_string=error_string;
}

void SerialFrameError::log_error()
{
	*log << logERROR << "Serial framing error - " << error_string << logENDL;
}

SerialFrame::SerialFrame(unsigned char* frame_data, unsigned char frame_size) // for receiving
{
	int i;
	//frame_id=(frame_data[0] & 0xF8) >> 3;
	payload_size=frame_data[0];
	//payload_size=(frame_data[0] & 0x07);
	//checksum=frame_data[frame_size-1];
	instruction=frame_data[1];
//	unsigned char c=0;
	if ((payload_size+2)!=frame_size)
	{
		throw (SerialFrameError("Incorrect length frame received"));
	}
	//for (int i=1;i<frame_size-1;i++)
	//	c+=frame_data[i];
	//if (c!=checksum)
	//	throw (SerialFrameError("Incorrect checksum received"));	
	payload=new unsigned char[payload_size];
	memcpy((void *)payload,(void *)(frame_data+2),payload_size);
	//for (i=0;i<payload_size;i++)
	//	payload[i]=frame_data[2+i];
	log=Logger::get_singleton().get_category_ptr("SerialFramer");
	
}

SerialFrame::SerialFrame(unsigned char instruction, unsigned char* payload, unsigned char payload_size)
{
	frame_id=0;
	this->instruction=instruction;
	this->payload=payload;
	this->payload_size=payload_size;
}

SerialFrame::~SerialFrame()
{
	delete[] payload;
}

void SerialFrame::transmit(SerialHandler *serialhandler)
{
	unsigned char* buffer;
	buffer=new unsigned char[payload_size+4];
	buffer[0]=START_FLAG;
	buffer[1]=payload_size;
	buffer[2]=instruction;
	memcpy((void*)&buffer[3],(void*)payload,payload_size);
	//for (int i=0;i<payload_size;i++)
	//	buffer[3+i]=payload[i];
	buffer[payload_size+3]=STOP_FLAG;
	serialhandler->transmit(buffer,payload_size+4);
	delete[] buffer;
}

double SerialRobotInterface::sanitize_compass(int prevbearing, int bearing)
{
	int actualvalue;
	if (bearing-prevbearing>15)
	{
		actualvalue=bearing;
	}
	else if (prevbearing-bearing>15)
	{
		actualvalue=prevbearing;
	}
	else
	{
		actualvalue=bearing;
	}
	if (compass_mapping)
	{
		if (actualvalue<compassmin) actualvalue=compassmin;
		if (actualvalue>compassmax) actualvalue=compassmax;
		return map_values(compassmin,compassmax,0.0,360.0,actualvalue);
	}
	else return double(actualvalue);
}

void SerialFrame::process(SerialRobotInterface *serialrobot)
{
	static int prevcompass=0;
	int curcompass;
	int actualvalue;
	//*log << logINFO << "Instruction = " << int(instruction) << logENDL;
	//*log << logINFO << "Frame id    = " << int(frame_id) << logENDL;
	//*log << logINFO << "Payload size= " << int(payload_size) << logENDL;
	//*log << logINFO << "Payload     = " << logENDL;
	//for (int i=0;i<payload_size;i++) *log << logINFO << int(payload[i]) << logENDL;
//	*log << logINFO << "Checksum    = " << int(checksum) << logENDL;
	switch (instruction)
	{
		case SONAR_DATA:
			if (payload_size!=5) throw SerialFrameError("Incorrect payload size for sonar data");
			int* distances;
			distances=new int[4];
			//memcpy((void*)distances,(void*)payload,4);
			distances[0]=int(payload[0]);
			distances[1]=int(payload[1]);
			distances[2]=int(payload[2]);
			distances[3]=int(payload[3]);
			serialrobot->on_sonar_data_rx(int(payload[4]),distances);
			delete[] distances;
			break;
		case COMPASS_DATA:
			if (payload_size!=2) throw SerialFrameError("Incorrect payload size for compass data");
			curcompass=guint16((payload[1])<<7)+payload[0];
			serialrobot->on_compass_data_rx(SerialRobotInterface::get_singleton().sanitize_compass(prevcompass,curcompass));
			prevcompass=curcompass;
			break;
		default:
			*log << logERROR << "Unexpected packet type." << logENDL;
	}
}

int SerialFrame::get_frame_id()
{
	return frame_id;
}
