#ifndef EXECUTION_ENGINE_H
#define EXECUTION_ENGINE_H

#include <string>
#include "IoRedir.h"
#include "ComradeFrontEnd.h"
#include "LocalisationEngine.h"
#include "EvidenceGrid.hh"
#include "SectorDraw.hh"
#include "Constants.hh"
#include "SensorStructure.h"
#include "Singleton.hh"
#include "ObstacleAvoider.h"
//#include "MotorController.h"
#include "OdometryEngine.h"
#include "LineFollower.h"
#include "CIRC.hh"

namespace Comrade {
	namespace ComradeRuntime {

enum ExecutionMode {EM_STOP,EM_TELEOPERATION, EM_VFH, EM_LINEFOLLOW};		
		
class ExecutionEngine: public Comrade::Corelib::Singleton<ExecutionEngine>
{
protected:	bool busy;
		AbstractSensorModel sensor_pack;
		
		Comrade::Corelib::EvidenceGrid<unsigned char>* evidence_grid;
		Comrade::Corelib::EvidenceGrid<unsigned char>* evidence_grid_vfh;
		Comrade::Corelib::EvidenceGrid<unsigned char>* local_grid;
		
		Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>* img_buf;
		
		Comrade::Corelib::SectorDraw sectordraw;
		ExecutionMode mode;
		sigc::connection linefollow_conn;
		sigc::connection vfh_conn;
		sigc::connection teleoperation_conn;

public:		ComradeFrontEnd* comrade_vm;
		LocalisationEngine localiser;
		ObstacleAvoider *obstacle_avoider;
		//MotorController *motor_controller; // Has been moved into AbstractHardware.h
		OdometryEngine *odometry_engine;
		LineFollower line_follower;
		Comrade::Corelib::CIRC circ;

		ExecutionEngine(ComradeFrontEnd* cfe);

		void assign_vm(ComradeFrontEnd* vm);
		void show_copyright();
		void activate();

		void update_compass_data(int bearing);
		void update_sonar_data(int bearing,int* array);

		void start_vfh();
		void stop_vfh();

		void start_linefollow();
		void stop_linefollow();

		void start_teleoperation();
		void stop_teleoperation();
		
		void circ_start(unsigned char mode);
		void circ_set_speeds(Comrade::Corelib::speed_update& su);

		bool line_fn();
		std::pair<int,int> circle_fn();
		void build_stereomap();

		int sonar_age;

		~ExecutionEngine();
};

}
}

#endif
