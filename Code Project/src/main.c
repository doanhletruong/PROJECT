#include <avr/io.h>
#include <util/delay.h>
#include "pins.h"
#include "dht.h"
#include "lcd.h"
#include "servo.h"

// Function prototypes
static void init_gpio(void);
static void init_buzzer(void);
static void set_buzzer(uint16_t frequency);
static uint8_t read_buttons(void);

int main(void) {
    float temperature;
    
    // Initialize all peripherals
    init_gpio();
    init_buzzer();
    servo_init();
    lcd_init(LCD_ADDRESS);
    dht_setup(DHT_PIN);
    
    // Enable LCD backlight
    lcd_backlight();
    
    while (1) {
        // Read temperature
        temperature = dht_get_temperature();
        
        // Update LCD display
        lcd_clear();
        lcd_set_cursor(0, 0);
        lcd_print("Temperature:");
        lcd_set_cursor(12, 0);
        lcd_print_num(temperature);
        lcd_print_char(223);  // Degree symbol
        lcd_print("C");
        
        // Set initial LED states
        PORTD |= (1 << LED1_PIN);  // LED1 HIGH
        PORTB |= (1 << LED0_PIN);  // LED0 HIGH
        
        // Read button states
        uint8_t buttons = read_buttons();
        uint8_t button1_pressed = (buttons & 0x01);
        uint8_t button2_pressed = (buttons & 0x02);
        uint8_t button3_pressed = (buttons & 0x04);
        
        // Clear states for LED2, LED3, and bulb
        PORTD &= ~((1 << LED2_PIN) | (1 << LED3_PIN));
        PORTB &= ~(1 << BULB_PIN);
        
        // Temperature control logic
        if ((temperature > TEMP1 && temperature < TEMP2) || button1_pressed) {
            servo_write(90);
            PORTD |= (1 << LED3_PIN);     // LED3 HIGH
            PORTD &= ~(1 << LED2_PIN);    // LED2 LOW
            PORTB &= ~(1 << BULB_PIN);    // BULB LOW
            set_buzzer(0);                // Stop buzzer
            _delay_ms(1000);
        }
        else if ((temperature < TEMP1 && temperature > TEMP3) || button2_pressed) {
            servo_write(0);
            PORTD &= ~(1 << LED3_PIN);    // LED3 LOW
            PORTD |= (1 << LED2_PIN);     // LED2 HIGH
            PORTB |= (1 << BULB_PIN);     // BULB HIGH
            set_buzzer(FREQUENCY1);
        }
        else if ((temperature < TEMP3 && temperature > TEMP4) || 
                 (temperature > TEMP2 && temperature < TEMP5) || 
                 button3_pressed) {
            servo_write(0);
            PORTD &= ~((1 << LED3_PIN) | (1 << LED2_PIN));  // LED3 & LED2 LOW
            PORTB &= ~(1 << BULB_PIN);    // BULB LOW
            set_buzzer(FREQUENCY2);
        }
        else {
            servo_write(0);
            PORTD &= ~((1 << LED3_PIN) | (1 << LED2_PIN));  // LED3 & LED2 LOW
            PORTB &= ~(1 << BULB_PIN);    // BULB LOW
            set_buzzer(0);                // Stop buzzer
        }
        
        _delay_ms(500);
    }
    
    return 0;
}

static void init_gpio(void) {
    // Configure LED pins as outputs
    DDRD |= (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN);
    DDRB |= (1 << LED0_PIN) | (1 << BULB_PIN);
    
    // Configure button pins as inputs with pull-up
    DDRD &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN) | (1 << BUTTON3_PIN));
    PORTD |= (1 << BUTTON1_PIN) | (1 << BUTTON2_PIN) | (1 << BUTTON3_PIN);
}

static void init_buzzer(void) {
    // Configure buzzer pin as output
    DDRB |= (1 << BUZZER_PIN);
}

static void set_buzzer(uint16_t frequency) {
    if (frequency == 0) {
        PORTB &= ~(1 << BUZZER_PIN);
    } else {
        // Simple tone generation
        uint16_t delay = 500000 / frequency;  // Half period in microseconds
        PORTB |= (1 << BUZZER_PIN);
        _delay_us(delay);
        PORTB &= ~(1 << BUZZER_PIN);
        _delay_us(delay);
    }
}

static uint8_t read_buttons(void) {
    uint8_t buttons = 0;
    
    // Read button states (active low due to pull-up)
    if (!(PIND & (1 << BUTTON1_PIN))) buttons |= 0x01;
    if (!(PIND & (1 << BUTTON2_PIN))) buttons |= 0x02;
    if (!(PIND & (1 << BUTTON3_PIN))) buttons |= 0x04;
    
    return buttons;
}
