#include "OdometryEngine.h"

using namespace Comrade::ComradeRuntime;
using namespace Comrade::Corelib;

OdometryEngine::OdometryEngine()
{
	x=400.0;
	y=300.0;
	orientation=0.0;
	//mouse_odometer.on_mouse_data_receive.connect(sigc::mem_fun(this,&OdometryEngine::mouse_data_receive));
}

OdometryEngine::~OdometryEngine()
{
}

void OdometryEngine::mouse_data_receive(int left, int right)
{
	double dist=(left+right)/2;
}

double OdometryEngine::get_x() const
{
	return x;
}

double OdometryEngine::get_y() const
{
	return y;
}

double OdometryEngine::get_orientation() const
{
	return orientation;
}
