#ifndef SERIAL_INSTRUCTIONS
#define SERIAL_INSTRUCTIONS

// #defines are used because the same file can be used in the microcontroller code 

#define START_FLAG 		0xFF
#define STOP_FLAG 		0xFE

// connection management
//#define HELLO			0xFD
//#define WELCOME			0xEF

#define INIT			0x00

// flow control
#define START_TX		0xEE
#define STOP_TX			0xDD

// acknowledgements
#define ACK			0x01
#define NACK			0x02 // data = n bytes containing frame id to resend.
#define PING			0x03 // no data
#define PONG			0x04 // no data

#define START_EVERYTHING	0x05 // no data
#define STOP_EVERYTHING		0x06 // no data

// sonar/sonar stepper related
#define SET_STEPS_PER_ANGLE	0x10 // data = 1 byte containing no of stepper motor steps per angle
#define SET_NO_OF_ANGLES	0x11 // data = 1 byte containing total no "angles" to sweep
#define SET_STEP_INTERVAL	0x12 // data = 1(?) byte containing a value which sets the time interval between steps
#define STEP_STEPPER		0x13 // data = 1 byte containing no of angles to step, most significant bit indicates direction
#define PING_SONAR		0x14 // no data
#define ROTATE_PING_SONAR	0x15 // no data, it rotates and then pings. (once)
#define SWEEP_PING_SONAR	0x16 // no data, start sweeping forward and backward and pinging

// compass
#define GET_COMPASS_INFO	0x20 // no data, send back a single compass data packet
#define START_AUTOSEND_COMPASS	0x21 // no data, start sending back compass data whenever it changes
#define STOP_AUTOSEND_COMPASS	0x22 // no data, stop autosend

// steering
#define STEER			0x30 // 2 bytes, first one left wheel, second one right wheel
				     // each byte has most significant bit as the direction, and
				     // the remaining as speed.
				     // The speed will thus be between 0-127 and has to be divided
				     // by the microcontroller according to the number of available
				     // speeds.
//#define GET_STEER_INFO	0x32 // what?

#define CAMERA_TRANSLATE	0x40 // complicated scheme
#define CAMERA_ROTATE		0x41 // complicated scheme

#define SONAR_START_CALIBRATE	0x50 // data = 1 byte containing the angle
#define SONAR_STOP_CALIBRATE	0x51 // no data

// robotic arm - to be implemented later
//#define ROT_ARM0	0x40
//#define ROT_ARM1	0x41
//#define ROT_ARM2	0x42
//#define JAW_STATE	0x43
//#define GET_ARM0_STATE	0x44
//#define GET_ARM1_STATE	0x45
//#define GET_ARM2_STATE	0x46
//#define GET_JAW_STATE	0x47
//#define GET_ARM_STATE	0x48

// the following payload is data of type
#define SONAR_DATA	0x81 // 5 data bytes - 4 sonar distances in cm + 1 data byte angle
#define COMPASS_DATA	0x82 // 2 data bytes - lower byte, higher byte
//#define STEER_DATA	0x83
//#define R_ARM0_DATA	0x85
//#define R_ARM1_DATA	0x86
//#define R_ARM2_DATA	0x87
//#define R_JAW_STATE	0x88
//#define R_ARM_STATE	0x89


#endif
