#include "Logger.hh"
#include "Configurator.hh"

#include <iostream>

using namespace std;
using namespace Comrade::Corelib;
using namespace log4cpp;


Logger::Logger()
{
	Configurator &c=Configurator::get_singleton();
	appender=new FileAppender("FileAppender",c.get_logger_filename());
//	stdoutappender=new OstreamAppender("StdoutAppender",&std::cout);
	layout=new BasicLayout();
	appender->setLayout(layout);
	Priority::Value v=Priority::getPriorityValue(c.get_logger_priority());
	Category& rootcat=Category::getRoot();
	rootcat.setPriority(v);
}

Logger::~Logger()
{
	Category::shutdown();
	delete appender;
//	delete stdoutappender;
}

/** \brief Returns a log4cpp::Category by name
 *
 * Returns a \c log4cpp::Category which you can use to log error/debug messages.
 *
 * @param name The name of the category (will appear in the log messages)
 * @return The \c log4cpp::Category which can be used to log messages
 */
Category& Logger::get_category(string name)
{
	Category &c=Category::getInstance(name);
	c.removeAllAppenders();
	c.setAdditivity(false);
	c.setAppender(*appender);
//	c.setAppender(*stdoutappender);
	return c;	
}

Category* Logger::get_category_ptr(string name)
{
	return &(get_category(name));
}
