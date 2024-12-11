#include "servo.h"
#include "pins.h"
#include <avr/io.h>
#include <util/delay.h>

// Timer1 PWM settings for servo control
// For 50Hz servo control signal:
// 16MHz/(8 * (1 + ICR1)) = 50Hz
// ICR1 = 39999
#define SERVO_MIN    2000    // 0.5ms pulse (0 degrees)
#define SERVO_MAX    4000    // 2.5ms pulse (180 degrees)

void servo_init(void) {
    // Configure Timer1 for PWM operation
    DDRB |= (1 << SERVO_PIN);  // Set servo pin as output
    
    // Set up Timer1 in Fast PWM mode
    TCCR1A = (1 << COM1A1) | (1 << WGM11);  // Clear on Compare Match, Fast PWM
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);  // Fast PWM, Prescaler = 8
    
    ICR1 = 39999;  // Set PWM frequency to 50Hz
    
    // Initialize servo to 0 degrees
    servo_write(0);
}

void servo_write(uint8_t angle) {
    uint16_t pulse;
    
    // Constrain angle to 0-180 degrees
    if (angle > 180) {
        angle = 180;
    }
    
    // Convert angle to pulse width
    pulse = SERVO_MIN + (((SERVO_MAX - SERVO_MIN) * (uint32_t)angle) / 180);
    
    // Update PWM duty cycle
    OCR1A = pulse;
}
