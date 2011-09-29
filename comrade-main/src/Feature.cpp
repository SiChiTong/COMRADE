#include "Feature.h"

extern Comrade::IO::IO_Redirector stream;

Comrade::Osiris::Feature::Feature()
{
    identifier="NULL";
}

void Comrade::Osiris::Feature::set_identifier(std::string id)
{
    identifier=id;
}

std::string Comrade::Osiris::Feature::get_identifier()
{
    return identifier;
}

Comrade::Osiris::Feature::~Feature()
{}

Comrade::Osiris::USB_CameraFeature::USB_CameraFeature()
{
    identifier="Camera";

	#ifdef USING_LINUX
	temp_frame=frame_new(320,240,VIDEO_PALETTE_RGB24);
	grabber_ptr=NULL;
	stream.write("Running for the first time...\n");

	if (is_present()==true)
	{
		stream.write("Camera is active...\n");
	}

	else
	{
		stream.write("Camera is not present...\n");
	}
	#endif
}

bool Comrade::Osiris::USB_CameraFeature::is_present()
{
    stream.write("Checking for camera...\n");

	#ifdef USING_LINUX
	if (grabber_ptr!=NULL)
	{	
		fg_close(grabber_ptr);
	}

	grabber_ptr=fg_open("/dev/video");

	if (grabber_ptr!=NULL)
	{
		stream.write("Found a videostream device...\n");
		fg_set_format(grabber_ptr,VIDEO_PALETTE_RGB24);
		fg_set_capture_window(grabber_ptr,0,0,320,240);
		return true;
	}

	else
	{
		stream.write("Videostream device not found...\n");
		return false;
	}
	#endif

	#ifndef USING_LINUX
	stream.write("Dummy non-Linux return value...\n");

	return true;
	#endif
}

Comrade::Osiris::USB_CameraFeature::~USB_CameraFeature()
{
	#ifdef USING_LINUX
	if (grabber_ptr!=NULL)
	{
		fg_close(grabber_ptr);
	}
	
	#endif
}

Comrade::Osiris::ArmFeature::ArmFeature()
{
    identifier="Arm";
}

bool Comrade::Osiris::ArmFeature::is_present()
{
    stream.write("Checking for arm...\n");

    // Insert microcontroller interface code here...

    return false;
}

Comrade::Osiris::ArmFeature::~ArmFeature()
{}

/*Comrade::Osiris::ActuatorFeature::ActuatorFeature()
{
    identifier="Actuator(s)";
}

bool Comrade::Osiris::ActuatorFeature::is_present()
{
    stream.write("Checking for actuator(s)...\n");

    // Insert microcontroller interface code here...

    return true;
}

Comrade::Osiris::ActuatorFeature::~ActuatorFeature()
{}*/

// Test code: DO NOT ALTER
/*
int main()
{
    CameraFeature cam;

    cout<<cam.get_identifier();
	if (cam.set_active(true)==true)
	{
		cout<<"Active...\n";
	}
	
    cin.get();
    return 0;
}*/
