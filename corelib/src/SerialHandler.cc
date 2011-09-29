#include "SerialHandler.hh"
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <cerrno>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <cstdio>   /* Standard input/output definitions */
#include <cstring>  /* String function definitions */
#include <iostream>

#include "Configurator.hh"
#include "Logger.hh"
#include <glibmm/main.h>

using namespace std;
using namespace Comrade::Corelib;

SerialHandler::SerialHandler()
{
	missed_acks=0;
	log=Logger::get_singleton().get_category_ptr("SerialHandler");
	Configurator &c=Configurator::get_singleton();
	
	device = c.get_serial_device();
	rxbuffersize = c.get_serial_rxbuffersize();
	rxbuffer = new unsigned char[rxbuffersize];
	
	*log << logINFO << "serial rxbuffer size = " << rxbuffersize <<logENDL;
	fd = open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		*log << logERROR << "open_port: Unable to open " << device << logENDL;
	}
	else
	{
		*log << logINFO << "opened " << device << logENDL;
		fcntl(fd, F_SETFL, 0);
		
		tcgetattr(fd, &options);
		int serialspeed=c.get_serial_baudrate();
		int baudrate=int2baud(serialspeed);
		cfsetispeed(&options, baudrate);
    		cfsetospeed(&options, baudrate);
		options.c_cflag |= (CLOCAL | CREAD);
		*log << logINFO << "speed = " << serialspeed << " baud" << logENDL;
		
		/* 8N1 */
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		options.c_cflag &= ~CSIZE;
		options.c_cflag |= CS8;
		*log << logINFO << "using 8N1" << logENDL;
		
		/* enable hardware flow control */
		options.c_cflag &= ~CRTSCTS;
		options.c_iflag &= ~(IXON | IXOFF | IXANY);
		*log << logINFO << "enabled flow control" << logENDL;
		
		/* use raw input */
		options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
		
		/* use raw output */
		options.c_oflag &= ~OPOST;
		*log << logINFO << "using raw i/o" << logENDL;
		
		tcsetattr(fd, TCSANOW, &options);
		*log << logDEBUG << "set all serial options" << logENDL;
		
		tcgetattr(fd,&options);
		*log << logDEBUG << "flow control: " << (options.c_cflag & CRTSCTS) << logENDL;
		
		Glib::signal_io().connect(sigc::mem_fun(*this,&SerialHandler::receiver), fd, Glib::IO_IN);
		*log << logDEBUG << "listening on " << device << logENDL;
	}
}

SerialHandler::~SerialHandler()
{
	*log << logDEBUG << "closing port " << device << logENDL;
	close(fd);
	delete[] rxbuffer;
}

int SerialHandler::transmit(const unsigned char* data, int datasize)
{
	int n;
	n = write(fd, (const void*)data, datasize);
//	for (int i=0;i<datasize;i++)
	//{
//		*log << logDEBUG << int(data[i]) << " ";
	//}
	//*log << logENDL;
//	if (n < 0)
//		*log << logERROR << "serial data not sent" << logENDL;
	missed_acks++;
	return(n);
}

bool SerialHandler::receiver(Glib::IOCondition)
{
	int n;
	n = read(fd, &rxbuffer[0], rxbuffersize);
	if(n > 0)
	{
		on_serial_data_receive(rxbuffer, n);
	}
	else if(n < 0)
		*log << logERROR << "serial read() returned " << n << logENDL;
	return true;
}

int SerialHandler::int2baud(int speed)
{
	switch (speed)
	{
		case 300:
			return B300;
		case 1200:
			return B1200;
		case 2400:
			return B2400;
		case 4800:
			return B4800;
		case 9600:
			return B9600;
		case 19200:
			return B19200;
		case 38400:
			return B38400;
		case 57600:
			return B57600;
		case 115200:
			return B115200;
		default:
			return B19200;
	}
}
