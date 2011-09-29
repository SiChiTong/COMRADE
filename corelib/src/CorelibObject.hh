#ifndef COMRADE_CORELIB_CORELIBOBJECT
#define COMRADE_CORELIB_CORELIBOBJECT

#include "Configurator.hh"
#include "Logger.hh"
#include "Singleton.hh"

namespace Comrade
{
	
namespace Corelib
{
	
class CorelibObject: public Singleton<CorelibObject>
{
	public:
		CorelibObject();
		~CorelibObject();
	private:
		Configurator *configurator;
		Logger *logger;
		
};
	
} // namespace Corelib

} // namespace Comrade

#endif
