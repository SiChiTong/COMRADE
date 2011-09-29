#include "MouseOdometer.hh"

using namespace std;
using namespace Comrade::Corelib;

MouseOdometer::MouseOdometer()
{
	log=Logger::get_singleton().get_category_ptr("MouseOdometer");
	Configurator &c=Configurator::get_singleton();
	device = c.get_mouse_device();
	exchange_lr = c.get_mouse_exchange_lr();
	leftinvert = c.get_mouse_leftinvert();
	rightinvert = c.get_mouse_rightinvert();
	
  	datapos = 0;
  	window = c.get_mouse_updateinterval();
  	hticks = vticks = 0;
  
	fd = open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd < 0) 
	{
		*log << logERROR << "Could not open mouse on " << device << logENDL;
	}
  
	tcgetattr(fd,&oldoptions);       // save port settings 
  
	newoptions.c_cflag = B1200 | CS7 | CLOCAL | CREAD | HUPCL;
	newoptions.c_iflag = IGNBRK | IGNPAR;
	newoptions.c_lflag = ~(ICANON | ECHO | ECHOE | ISIG);    // raw mode
	newoptions.c_cc[VTIME] = 0;  // shouldn't matter due to O_NDELAY
	newoptions.c_cc[VMIN] = 1; 
	
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newoptions);
  
	packetsize=3;
	readsize=1;
	proto[0]=0xc0;
	proto[1]=0x00;
	proto[2]=0x00;
	proto[3]=0x00;
  
	gettimeofday(&before, NULL);
	now = before;
	
	Glib::signal_io().connect(sigc::mem_fun(*this,&MouseOdometer::read_data), fd, Glib::IO_IN);

}

MouseOdometer::~MouseOdometer()
{
	tcsetattr(fd,TCSANOW,&oldoptions); 
	close(fd);
}

bool MouseOdometer::read_data(Glib::IOCondition)
{ 
	/*
	 *             D6    D5    D4    D3    D2    D1    D0
	 *
	 * 1st byte    1     LB    RB    Y7    Y6    X7    X6
	 * 2nd byte    0     X5    X4    X3    X2    X1    X0
	 * 3rd byte    0     Y5    Y4    Y3    Y2    Y1    Y0
	 */

	gettimeofday(&now, NULL);
	if (datapos < 0)
	{
		datapos++;
		return true;
	}
	
	//read first byte
	if(!read(fd, buffer, 1))
	{
		*log << logERROR << "Couldn't read first byte" << logENDL;
	}
	
	//check to make sure it the "first" packet 
	if (buffer[0] & proto[0] != proto[1])
	{
		*log << logERROR << "Not at beginning of packet" << logENDL;
		return true;
	}
	else  //read first byte
	{
		datapos = readbytes = 1;
	}
	
	//read rest of the packet
	while(datapos < packetsize)
	{
		readbytes = read(fd, buffer + datapos, readsize);
		datapos += readbytes;
	}
	
	//not really sure why this works but it does
	//I think the ps2 shoots an extra packet
	//  if(!strcmp("ps2", mouseType_))
	//datapos = -1;
	datapos = 0;
	
	if (buffer[1] != 0)
		hticks += (buffer[0] & 0x10) ? buffer[1] - 256 : buffer[1];
	
	if (buffer[2] != 0)
		vticks += (buffer[0] & 0x20) ? buffer[2] - 256 : buffer[2];
	
	if (((now.tv_sec - before.tv_sec) * 1000.0 + (now.tv_usec - before.tv_usec) / 1000.0) >= window)
	{
		
		if (exchange_lr)
		{
			int temp=vticks;
			vticks=hticks;
			hticks=temp;
		}
		if (leftinvert) hticks=-hticks;
		if (rightinvert) vticks=-vticks;
		on_mouse_data_receive(hticks,vticks);
		hticks = vticks = 0;
		before = now;
	}
	return true;
}
