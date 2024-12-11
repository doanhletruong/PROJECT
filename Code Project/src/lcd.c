#include "lcd.h"
#include "pins.h"
#include <avr/io.h>
#include <util/delay.h>

#define I2C_START    0
#define I2C_DATA     1
#define I2C_STOP     2

static uint8_t lcd_addr;
static uint8_t displayfunction;
static uint8_t displaycontrol;
static uint8_t displaymode;
static uint8_t backlightval = 0x08;

// I2C functions
static void i2c_init(void) {
    TWSR = 0;
    TWBR = ((F_CPU/100000UL)-16)/2;  // Set bit rate for 100kHz
    TWCR = (1<<TWEN);                // Enable TWI
}

static void i2c_start(void) {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

static void i2c_stop(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    _delay_us(100);  // Wait for stop condition to be executed
}

static void i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

static void lcd_write_4bits(uint8_t value) {
    i2c_start();
    i2c_write((lcd_addr << 1) | I2C_DATA);
    i2c_write(value | backlightval);
    i2c_write((value | 0x04) | backlightval);  // Enable pulse
    _delay_us(1);
    i2c_write((value & ~0x04) | backlightval); // Enable pulse end
    i2c_stop();
}

static void lcd_send(uint8_t value, uint8_t mode) {
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;
    lcd_write_4bits((highnib) | mode);
    lcd_write_4bits((lownib) | mode);
}

static void lcd_command(uint8_t value) {
    lcd_send(value, 0);
    _delay_us(50);
}

void lcd_init(uint8_t addr) {
    lcd_addr = addr;
    displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
    
    i2c_init();
    
    // Wait for LCD to power up
    _delay_ms(50);
    
    // Initialize 4-bit mode
    lcd_write_4bits(0x03 << 4);
    _delay_ms(5);
    lcd_write_4bits(0x03 << 4);
    _delay_ms(5);
    lcd_write_4bits(0x03 << 4);
    _delay_us(150);
    lcd_write_4bits(0x02 << 4);
    
    lcd_command(LCD_FUNCTIONSET | displayfunction);
    
    displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    lcd_command(LCD_DISPLAYCONTROL | displaycontrol);
    
    lcd_clear();
    
    displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    lcd_command(LCD_ENTRYMODESET | displaymode);
    
    lcd_command(LCD_RETURNHOME);
    _delay_ms(2);
}

void lcd_clear(void) {
    lcd_command(LCD_CLEARDISPLAY);
    _delay_ms(2);
}

void lcd_backlight(void) {
    backlightval = 0x08;
    lcd_write_4bits(0);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcd_print_char(char c) {
    lcd_send(c, 0x01);
    _delay_us(50);
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_print_char(*str++);
    }
}

void lcd_print_num(float num) {
    char buf[16];
    int int_part = (int)num;
    int dec_part = (int)((num - int_part) * 10);
    
    // Handle negative numbers
    if (num < 0) {
        lcd_print_char('-');
        int_part = -int_part;
        dec_part = -dec_part;
    }
    
    // Convert to string and print
    sprintf(buf, "%d.%d", int_part, dec_part);
    lcd_print(buf);
}
