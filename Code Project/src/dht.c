#include "dht.h"
#include <avr/io.h>
#include <util/delay.h>
#include "pins.h"

static uint8_t dht_pin;
static uint8_t dht_data[5];

void dht_init(void) {
    // Set DHT pin as input initially
    DDRC &= ~(1 << DHT_PIN);
}

void dht_setup(uint8_t pin) {
    dht_pin = pin;
    dht_init();
}

static uint8_t read_dht_data(void) {
    uint8_t i, j, result = 0;
    
    // Wait for response
    for (i = 0; i < 50; i++) {
        if (!(PINC & (1 << DHT_PIN))) {
            break;
        }
        _delay_us(1);
    }
    
    // Read 40 bits (5 bytes)
    for (i = 0; i < 5; i++) {
        result = 0;
        for (j = 0; j < 8; j++) {
            // Wait for rising edge
            while (!(PINC & (1 << DHT_PIN)));
            
            _delay_us(30);
            
            if (PINC & (1 << DHT_PIN)) {
                result |= (1 << (7-j));
            }
            
            // Wait for falling edge
            while (PINC & (1 << DHT_PIN));
        }
        dht_data[i] = result;
    }
    
    return ((dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) == dht_data[4]);
}

float dht_get_temperature(void) {
    uint16_t temperature;
    
    // Start signal
    DDRC |= (1 << DHT_PIN);    // Output mode
    PORTC &= ~(1 << DHT_PIN);  // Pull low
    _delay_ms(18);             // Wait at least 18ms
    PORTC |= (1 << DHT_PIN);   // Pull high
    _delay_us(40);             // Wait 40us
    DDRC &= ~(1 << DHT_PIN);   // Input mode
    
    if (read_dht_data()) {
        temperature = dht_data[2] & 0x7F;
        temperature *= 256;
        temperature += dht_data[3];
        if (dht_data[2] & 0x80) {
            temperature *= -1;
        }
        return temperature / 10.0;
    }
    
    return 0.0; // Return 0 if reading failed
}
