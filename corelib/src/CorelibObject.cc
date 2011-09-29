#include "CorelibObject.hh"

using namespace Comrade::Corelib;

CorelibObject::CorelibObject()
{
	configurator=new Configurator;
	configurator->open_file("comrade.xml");
	logger=new Logger;
}

CorelibObject::~CorelibObject()
{
	delete logger;
	delete configurator;
}
