#include "ParallelRobot.hh"

using namespace Comrade::Corelib;

ParallelRobot::ParallelRobot()
{
	pp.outputByte(0);
	c=Glib::signal_timeout().connect(SigC::slot(*this,&ParallelRobot::timeout_handler), 1);
}

ParallelRobot::~ParallelRobot()
{
}

void ParallelRobot::steer(int left, int right)
{
}

bool ParallelRobot::timeout_handler()
{
	return true;
}
