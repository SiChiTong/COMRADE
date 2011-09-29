#ifndef COMRADE_CORELIB_CIRC
#define COMRADE_CORELIB_CIRC

#include "sigc++/sigc++.h"
#include "Multicaster.hh"
#include "Logger.hh"

namespace Comrade
{
	
namespace Corelib
{

struct map_update
{
	double x;
	double y;
	double orientation;
	unsigned char sonar_angle;
	unsigned char sonar_data[4]; 
};	

struct speed_update
{
	int left, right;
};

enum CIRCPacketType {CIRC_MAPUPDATE, CIRC_SPEEDUPDATE, CIRC_START, CIRC_STOP, CIRC_SETSPEEDS, CIRC_CAMERAUPDATE};

class CIRC: public Singleton<CIRC>
{
	public:
		CIRC();
		~CIRC();
		void send_map_update(map_update& mu);
		void send_speed_update(speed_update& su);
		void send_set_speeds(speed_update& su);
		void send_start(unsigned char mode);
		void send_stop();
		void send_camera_update();
		sigc::signal<void,map_update&> on_map_update_receive;
		sigc::signal<void,speed_update&> on_speed_update_receive;
		sigc::signal<void,speed_update&> on_set_speeds_receive;
		sigc::signal<void> on_camera_update_receive;
		sigc::signal<void, unsigned char> on_start_receive;
		sigc::signal<void> on_stop_receive;
	private:
		Multicaster *multicaster;
		log4cpp::Category *log;		
		void receiver(unsigned char *data, int datasize);		
};

} // Corelib

} // Comrade

#endif
