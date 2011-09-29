#ifndef COMRADE_CORELIB_MOUSEODOMETER
#define COMRADE_CORELIB_MOUSEODOMETER

#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <sigc++/sigc++.h>
#include <glibmm/main.h>

#include "Singleton.hh"
#include "Logger.hh"
#include "Configurator.hh"

namespace Comrade
{
	
namespace Corelib
{

class MouseOdometer: public Singleton<MouseOdometer>, public sigc::trackable
{

	public:
		MouseOdometer();
		~MouseOdometer();
		sigc::signal<void,int,int> on_mouse_data_receive;

	private:

		log4cpp::Category *log;
		std::string device;
		int fd;
		unsigned char buffer[32];
		unsigned char proto[4];
		bool exchange_lr;
		termios oldoptions,newoptions;
		bool read_data(Glib::IOCondition);
	
		int datapos, packetsize, readsize, readbytes;
	  
		int hticks, vticks;
		long window;
		struct timeval before, now;
		bool leftinvert,rightinvert;
	
};

} // namespace Corelib

} // namespace Comrade

#endif
