#ifndef MULTICAST_SIGNAL_BUFFER_H
#define MULTICAST_SIGNAL_BUFFER_H

// NOTE: There should *not* be any dynamically allocated
// memory within any signal, since it is passed between
// machines.

#include <vector>
#include <cstring>
#include <string>
#include "CircularBuffer.h"
#include "ByteObjectConvertor.h"
#include "IoRedir.h"

namespace Comrade {
	namespace Osiris {

using namespace Comrade::IrisFoundation;

class Signal
{
protected:

public:		char signal_code[10];// Or maybe just an int will do...?

public:		Signal();
			~Signal();
};

class Signal_START: public Signal
{
protected:

public:		Signal_START();
			~Signal_START();
};

class Signal_STOP: public Signal
{
protected:

public:		Signal_STOP();
			~Signal_STOP();
};

class Signal_OBJ_DET: public Signal
{
protected:

public:		double line;
			double posx,posy;

public:		Signal_OBJ_DET();
			~Signal_OBJ_DET();
};

class MulticastSignalBuffer:
	public CircularBuffer<std::vector<char> >
{
protected:	int read_index;

public:		MulticastSignalBuffer(int sz=DEFAULT_BUFFER_SIZE);

			std::vector<char> read_and_discard();

			~MulticastSignalBuffer();
};

}
}

#endif


