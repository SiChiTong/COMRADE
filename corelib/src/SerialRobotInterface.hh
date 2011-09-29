#ifndef COMRADE_CORELIB_SERIALROBOT
#define COMRADE_CORELIB_SERIALROBOT

#include "RobotInterface.hh"
#include "SerialHandler.hh"
#include "Logger.hh"

#include <string>
#include <sigc++/sigc++.h>
#include <map>
#include <deque>

namespace Comrade
{
	
namespace Corelib
{
	
enum SerialFrameState {FRAME_NOT_STARTED, FRAME_STARTED};

class SerialRobotInterface;

class SerialFrame
{
	public:
		SerialFrame(unsigned char* frame_data, unsigned char frame_size);
		SerialFrame(unsigned char instruction, unsigned char* payload, unsigned char payload_size);
		~SerialFrame();
		void process(SerialRobotInterface *serialrobot);
		void transmit(SerialHandler *serialhandler);
		int get_frame_id();
	private:
		unsigned char instruction;
		unsigned char frame_id;
		unsigned char payload_size;
		unsigned char* payload;
		//unsigned char checksum;
		log4cpp::Category *log;
};

class SerialRobotInterface: public RobotInterface, public sigc::trackable
{
	public:
		SerialRobotInterface();
		~SerialRobotInterface();
		void start();
		void stop();
		void camera_translate(int angles);
		void camera_translate_set_steps_per_angle(int steps);
		void camera_rotate(int angles);
		void camera_rotate_set_steps_per_angle(int steps);
		void steer(int left, int right);
		void mapped_steer(int left, int right);
		void move_arm(int armno, double angle);
		void sonar_calibrate_start(int angle);
		void sonar_calibrate_stop();
		void initialize();
		void reload_config();
		void enable_compass_mapping(bool enable);
		double sanitize_compass(int prevbearing, int bearing);

	private:
		SerialHandler *serialhandler;
		log4cpp::Category *log;
		int maxframesize;
		unsigned char *framebuffer;
		int framebufferlen;
		std::map<int,SerialFrame *> inframecache,outframecache;
		std::deque<SerialFrame *> outframequeue;
		int nextframeid;
		bool leftinvert, rightinvert;
		int lfmax,rfmax, lbmax, rbmax, lfmin, rfmin, lbmin, rbmin;
		int compassmax, compassmin;
		sigc::connection timeout_conn;
		bool timeout_active;
		bool compass_mapping;
	
		void process_char(unsigned char c);
		void process_frame();
		void receiver(unsigned char* data, int data_size);
		bool send_next_packet();
		void queue_frame(unsigned char instruction, unsigned char* payload, unsigned char payload_size);
		void reattach_timeout();
		bool transmit_timeout();
		
		double map_values(double from_start, double from_end, double to_start, double to_end, double value);
};

class SerialFrameError
{
	public:
		SerialFrameError(string error_string);
		void log_error();
	private:
		std::string error_string;
		log4cpp::Category *log;
};
	
} // namespace Corelib

} // namespace Comrade

#endif
