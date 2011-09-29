#include "CIRC.hh"
#include "Configurator.hh"

using namespace Comrade::Corelib;

CIRC::CIRC()
{
	log=Logger::get_singleton().get_category_ptr("CIRC");
//	Configurator &c=Configurator::get_singleton();
	
	multicaster=new Multicaster;
	multicaster->on_data_receive.connect(sigc::mem_fun(*this,&CIRC::receiver));
}

void CIRC::send_map_update(map_update& mu)
{
	unsigned char* data;
	data=new unsigned char[sizeof(mu)+1];
	data[0]=CIRC_MAPUPDATE;
	memcpy((void *)(data+1),(void *)&mu,sizeof(mu));
	multicaster->transmit(data,sizeof(mu)+1);
	delete data;
}

void CIRC::send_speed_update(speed_update& su)
{
	unsigned char* data;
	data=new unsigned char[sizeof(su)+1];
	data[0]=CIRC_SPEEDUPDATE;
	memcpy((void *)(data+1),(void *)&su,sizeof(su));
	multicaster->transmit(data,sizeof(su)+1);
	delete data;
}

void CIRC::send_set_speeds(speed_update& su)
{
	unsigned char* data;
	data=new unsigned char[sizeof(su)+1];
	data[0]=CIRC_SETSPEEDS;
	memcpy((void *)(data+1),(void *)&su,sizeof(su));
	multicaster->transmit(data,sizeof(su)+1);
	delete data;
}

void CIRC::send_start(unsigned char mode)
{
	unsigned char* data;
	data=new unsigned char[2];
	data[0]=CIRC_START;
	data[1]=mode;
	multicaster->transmit(data,2);
	delete data;
}

void CIRC::send_stop()
{
	unsigned char data=CIRC_STOP;
	multicaster->transmit(&data,1);
}

void CIRC::send_camera_update()
{
	unsigned char data=CIRC_CAMERAUPDATE;
	multicaster->transmit(&data,1);
}

void CIRC::receiver(unsigned char *data, int datasize)
{
	if (datasize>0)
	{
		CIRCPacketType type=CIRCPacketType(data[0]);
		map_update mu;
		speed_update su;
		switch(type)
		{
			case CIRC_MAPUPDATE:
				memcpy((void *)&mu,(void *)(data+1),sizeof(mu));
				on_map_update_receive(mu);
				break;
			case CIRC_SPEEDUPDATE:
				memcpy((void *)&su,(void *)(data+1),sizeof(su));
				on_speed_update_receive(su);
				break;
			case CIRC_SETSPEEDS:
				memcpy((void *)&su,(void *)(data+1),sizeof(su));
				on_set_speeds_receive(su);
				break;
			case CIRC_START:
				on_start_receive(data[1]);
				break;
			case CIRC_STOP:
				on_stop_receive();
				break;
			case CIRC_CAMERAUPDATE:
				on_camera_update_receive();
			default:
				*log << logERROR << "Unknown packet type" << logENDL;
				break;				
		}
	}
}

CIRC::~CIRC()
{
	delete multicaster;
}
