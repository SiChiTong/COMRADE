#include <glibmm/main.h>
#include <iostream>
#include "MotorController.h"
#include <cmath>

using namespace Comrade::Osiris;
using namespace std;

MotorController::MotorController(Comrade::Corelib::RobotInterface *ri)
{
	this->ri=ri;
	curleft=0;
	curright=0;
	targetleft=0;
	targetright=0;
	Glib::signal_timeout().connect(sigc::mem_fun(this,&MotorController::timeout_handler), 50);
}

MotorController::~MotorController()
{
	ri->steer(0,0);
}

bool MotorController::is_present()
{
	return true;
}

bool MotorController::timeout_handler()
{
	curleft=calc_new_speed(curleft,targetleft);
	curright=calc_new_speed(curright,targetright);
	//cout << "left = " << curleft << " ("<<targetleft<<") right = " << curright<<" ("<<targetright<<")"<<endl;
	ri->steer(curleft,curright);	
	return true;
}

int MotorController::calc_new_speed(int old, int target)
{
	int inc;
	int diff=target-old;
	if (diff==0) 
		inc=0;
	else if (diff>20)
		inc=20;
	else if (diff<-20)
		inc=-20;
	else inc=diff;
	return old+inc;
}

void MotorController::set_target_direction(double dest_angle, double r)
{
	cout << "dest_angle = " << dest_angle << endl;
	dest_angle+=45.0;
	int right=int(r*cos((M_PI/180)*dest_angle));
	int left=int(r*sin((M_PI/180)*dest_angle));
	cout << "left = " << left << " right = " << right << endl;
	set_target_speeds(left,right);
}

void MotorController::set_target_speeds(int left, int right)
{
	cout << "left = " << left << " right = " << right << endl;
	targetleft=left;
	targetright=right;
}
