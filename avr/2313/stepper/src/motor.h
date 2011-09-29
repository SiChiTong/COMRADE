#ifndef DC_MOTOR
#define DC_MOTOR

#include <inttypes.h>
#include <avr/sfr_defs.h>
#include <avr/signal.h>

#define STEPPER_STOP	0
#define STEPPER_UP		1
#define STEPPER_DOWN	2

#define FORWARD			0x06					// states for h-bridge
#define REVERSE			0x09
#define STOP			0x0F

#define ANGLE_MASK		0x7F
#define DIRECTION_MASK	0x80

#define DIR_FORWARD		0x00
#define DIR_REVERSE		0x80

#define MOTOR_OFF		0x00
#define MOTOR_ON		0x01

extern volatile unsigned char timer_value;
extern volatile unsigned char angle_to_rotate;
extern volatile char stepper_direction;
extern volatile unsigned char steps_per_angle;

void run_motors();
SIGNAL(SIG_OVERFLOW0);
#endif
