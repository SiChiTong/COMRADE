#ifndef SERIAL_HANDLER
#define SERIAL_HANDLER

#include <sigc++/sigc++.h>
#include <glibmm/main.h>
#include <string>
#include <termios.h>

#include "Logger.hh"

namespace Comrade
{

namespace Corelib
{

class SerialHandler: public sigc::trackable
{
	public:
		SerialHandler();
		~SerialHandler();
		int transmit(const unsigned char* data, int datasize);
		sigc::signal<void,unsigned char*, int> on_serial_data_receive;
		int missed_acks;
	private:
		log4cpp::Category *log;
		std::string device;
		int fd;
		bool receiver(Glib::IOCondition);
		unsigned char* rxbuffer;
		int rxbuffersize;
		termios options;
	
		int int2baud(int speed);
};

}

}
#endif
