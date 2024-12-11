#ifndef LCD_H
#define LCD_H

#include <stdint.h>

// LCD commands
#define LCD_CLEARDISPLAY    0x01
#define LCD_RETURNHOME      0x02
#define LCD_ENTRYMODESET    0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_FUNCTIONSET     0x20
#define LCD_SETCGRAMADDR    0x40
#define LCD_SETDDRAMADDR    0x80

// LCD flags
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT          0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00

#define LCD_8BITMODE   0x10
#define LCD_4BITMODE   0x00
#define LCD_2LINE      0x08
#define LCD_1LINE      0x00
#define LCD_5x10DOTS   0x04
#define LCD_5x8DOTS    0x00

void lcd_init(uint8_t addr);
void lcd_clear(void);
void lcd_backlight(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(const char *str);
void lcd_print_char(char c);
void lcd_print_num(float num);

#endif // LCD_H
