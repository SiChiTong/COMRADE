#ifndef COMRADE_CORELIB_LOGGER
#define COMRADE_CORELIB_LOGGER

#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/BasicLayout.hh>

#include "Singleton.hh"

namespace Comrade
{

namespace Corelib
{

/** \brief Centralized logging of error and debug messages
 *
 * This class logs error and debug messages at a central location for the whole
 * program, and can be configured in the config file using parameters
 * \c config.logger.filename and \c config.logger.priority. 
 * 
 * To use it, access the instance of this using \c Logger::get_singleton(), and 
 * create a \c log4cpp::Category object using \c get_category(). Eg:
 * \code
 * Logger& l=Logger::get_singleton();
 * log4cpp::Category &log=l.get_category("myclass"); //just a name to identify which module is logging
 * log << log4cpp::Priority::ERROR << "This is an error message" << log4cpp::CategoryStream::ENDLINE;
 * log << log4cpp::Priority::WARN << "This is a warning message" << log4cpp::CategoryStream::ENDLINE;
 * \endcode
 *
 * More docs in the log4cpp documentation.
 */

class Logger: public Singleton<Logger>
{
	public:
		Logger();
		~Logger();
		log4cpp::Category& get_category(string name);
		log4cpp::Category* get_category_ptr(string name);
	private:
		log4cpp::Appender *appender;
//		log4cpp::OstreamAppender *stdoutappender;
		log4cpp::Layout *layout;
		
};
	
// OK, the following is very evil, but it's necessary to reduce the amount of
// typing required while entering log statements

#define logEMERG log4cpp::Priority::EMERG
#define logFATAL log4cpp::Priority::FATAL
#define logALERT log4cpp::Priority::ALERT
#define logCRIT log4cpp::Priority::CRIT
#define logERROR log4cpp::Priority::ERROR
#define logWARN log4cpp::Priority::WARN
#define logNOTICE log4cpp::Priority::NOTICE
#define logINFO log4cpp::Priority::INFO
#define logDEBUG log4cpp::Priority::DEBUG
#define logNOTSET log4cpp::Priority::NOTSET
#define logENDL log4cpp::CategoryStream::ENDLINE;

} // namespace Corelib

} // namespace Comrade


#endif
