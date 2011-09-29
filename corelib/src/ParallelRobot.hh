#ifndef COMRADE_CORELIB_PARALLELROBOT
#define COMRADE_CORELIB_PARALLELROBOT

#include "Robot.hh"
#include "ParallelPort.hh"
#include <sigc++/sigc++.h>

namespace Comrade
{
	
namespace Corelib
{

class ParallelRobot: public Robot
{
	public:
		ParallelRobot();
		~ParallelRobot();
		void steer(int left, int right);
	private:
		ParallelPort pp;
		SigC::Connection c;
};
	
} // namespace Corelib

} // namespace Comrade

#endif
