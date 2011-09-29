#include <sys/io.h>
#include <iostream>
#include <cstdlib>
#include "ParallelPort.hh"

using namespace std;
using namespace Comrade::Corelib;

ParallelPort::ParallelPort()
{
	if (ioperm(BASEPORT,2,1))
	{
		std::cerr << "Error opening port.\n";
		exit(1);
	}
	outputByte(0);
}

ParallelPort::~ParallelPort()
{
	if (ioperm(BASEPORT,2,0))
	{
		std::cerr << "Error closing port.\n";
		exit(1);
	}
}

void ParallelPort::outputByte(unsigned char byte)
{
	curoutput=byte;
	outb(byte,BASEPORT);
}

void ParallelPort::outputBit(int bitno, bool status)
{
	unsigned char outbyte;
	if (status)
	{ // Setting bitno to high
		outbyte=(curoutput | (1<<bitno));	
	}
	else
	{ // Setting bitno low
		outbyte=(curoutput & (255-(1<<bitno)));
	}
	outputByte(outbyte);
}

void ParallelPort::refreshInput()
{
	curinput=inb(BASEPORT+1);
}

bool ParallelPort::inputBit(int bitno)
{
	bitno+=3;
	bool status=((1<<bitno) & curinput);
	return ((bitno==7)?(!status):status);
}
