#include "MulticastSignalBuffer.h"

extern Comrade::IO::IO_Redirector stream;

Comrade::Osiris::Signal::Signal()
{
	strcpy(signal_code,"NONE");
}

Comrade::Osiris::Signal::~Signal()
{}

Comrade::Osiris::Signal_START::Signal_START()
{
	strcpy(signal_code,"START");
}

Comrade::Osiris::Signal_STOP::~Signal_STOP()
{}

Comrade::Osiris::Signal_STOP::Signal_STOP()
{
	strcpy(signal_code,"START");
}

Comrade::Osiris::Signal_START::~Signal_START()
{}

Comrade::Osiris::Signal_OBJ_DET::Signal_OBJ_DET()
{
	strcpy(signal_code,"OBJ_DET");
	line=10.0;
	posx=posy=15.8;
}

Comrade::Osiris::Signal_OBJ_DET::~Signal_OBJ_DET()
{}

Comrade::Osiris::MulticastSignalBuffer::MulticastSignalBuffer(int sz):
Comrade::Osiris::CircularBuffer<std::vector<char> >(sz)
{
    write_index=read_index=0;
	
	Signal some;

	for (int i=0; i<=size-1; ++i)
	{
		c_queue[i]=Comrade
				  ::Osiris
				  ::ByteObjectConvertor<Signal>
				  ::object_to_byte(&some);
	}

	stream.write("MulticastSignalBuffer ready...\n");
}

std::vector<char> Comrade::Osiris::
MulticastSignalBuffer::read_and_discard()
{
	Comrade::Osiris::Signal sig;
	std::vector<char> buf=c_queue[read_index];
	c_queue[read_index]=Comrade::Osiris::
						ByteObjectConvertor<Comrade::
						Osiris::Signal>::
						object_to_byte(&sig);

	++read_index;
	read_index%=size;

	return buf;
}

Comrade::Osiris::MulticastSignalBuffer::~MulticastSignalBuffer()
{}

