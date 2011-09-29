#ifndef COMRADE_CORELIB_ROBOTINTERFACE
#define COMRADE_CORELIB_ROBOTINTERFACE

#include <Singleton.hh>
#include <Logger.hh>
#include <glib/gtypes.h>
#include <sigc++/sigc++.h>

namespace Comrade
{
	
namespace Corelib
{

class RobotInterface:public Singleton<RobotInterface>
{
	public:
		RobotInterface();
		virtual ~RobotInterface();
		virtual void start()=0;
		virtual void stop()=0;
		virtual void camera_translate(int angles)=0;
		virtual void camera_translate_set_steps_per_angle(int steps)=0;
		virtual void camera_rotate(int angles)=0;
		virtual void camera_rotate_set_steps_per_angle(int steps)=0;
		virtual void steer(int left, int right)=0;
		virtual void mapped_steer(int left, int right)=0;
		virtual void move_arm(int armno, double angle)=0;
		virtual void sonar_calibrate_start(int angle)=0;
		virtual void sonar_calibrate_stop()=0;
		virtual void initialize()=0;
		virtual void reload_config()=0;
		virtual double sanitize_compass(int, int)=0;
		virtual void enable_compass_mapping(bool)=0;
		guint32 get_id();
	
		sigc::signal<void, int, int*> on_sonar_data_rx; // angle, 4 values
		sigc::signal<void, int> on_compass_data_rx;
		/*SigC::Signal1<void, guint16> on_rarm0_data_rx;
		SigC::Signal1<void, guint16> on_rarm1_data_rx;
		SigC::Signal1<void, guint16> on_rarm2_data_rx;
		SigC::Signal1<void, guint16> on_rjaw_data_rx;
		SigC::Signal1<void, guint16*> on_robot_arm_data_rx;*/

	private:
		log4cpp::Category *log;		
		guint32 robot_id;
};

} // namespace Corelib

} // namespace Comrade

#endif
