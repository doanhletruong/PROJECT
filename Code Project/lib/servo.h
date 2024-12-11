#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

// Servo control functions
void servo_init(void);
void servo_write(uint8_t angle);  // angle: 0-180 degrees

#endif // SERVO_H
