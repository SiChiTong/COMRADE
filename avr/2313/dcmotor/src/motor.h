#ifndef DC_MOTOR
#define DC_MOTOR

#include <inttypes.h>
#include <avr/sfr_defs.h>
#include <avr/signal.h>

#define FORWARD			0x06					// states for h-bridge
#define REVERSE			0x09
#define STOP			0x0F

#define SPEED_MASK		0x7F
#define DIRECTION_MASK	0x80

extern volatile unsigned char m1speed, m2speed;
extern volatile unsigned char m1dir, m2dir;

SIGNAL(SIG_OVERFLOW0);
#endif
