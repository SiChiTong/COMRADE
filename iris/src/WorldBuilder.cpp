#include "WorldBuilder.h"

extern Comrade::IO::IO_Redirector stream;

Comrade::Iris3D::WorldBuilder::WorldBuilder()
{
	stream.write("WorldBuilder object created...\n");
}

Comrade::Iris3D::WorldBuilder::~WorldBuilder()
{}

