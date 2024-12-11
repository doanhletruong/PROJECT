#ifndef PINS_H
#define PINS_H

// Pin Definitions
#define SERVO_PIN       PB1     // Digital pin 9
#define BUZZER_PIN     PB4     // Digital pin 12
#define BUTTON1_PIN    PD5     // Digital pin 5
#define BUTTON2_PIN    PD6     // Digital pin 6
#define BUTTON3_PIN    PD7     // Digital pin 7
#define BULB_PIN       PB0     // Digital pin 8
#define LED0_PIN       PB3     // Digital pin 11
#define LED1_PIN       PD4     // Digital pin 4
#define LED2_PIN       PD3     // Digital pin 3
#define LED3_PIN       PD2     // Digital pin 2
#define DHT_PIN        PC0     // Analog pin A0

// Temperature thresholds
#define TEMP1          28
#define TEMP2          35
#define TEMP3          15
#define TEMP4          8
#define TEMP5          45

// Buzzer frequencies
#define FREQUENCY1     1000
#define FREQUENCY2     3000

// I2C LCD Address
#define LCD_ADDRESS    0x27
#define LCD_COLS       16
#define LCD_ROWS       2

#endif // PINS_H
