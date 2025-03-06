#include "./lib/lcd1602.h"
#include<avr/io.h>
#include<util/delay.h>
#ifndef F_CPU
#define F_CPU   16000000UL
#endif
// Define ports and pins for the LCD control and data pins
#define RS_PIN    PD7
#define EN_PIN    PD6
#define D4_PIN    PB0
#define D5_PIN    PB1
#define D6_PIN    PB2
#define D7_PIN    PB3

// Create LCD object with user-defined pins and ports
LCD1602 lcd(PORTD, RS_PIN, PORTD, EN_PIN, PORTB, D4_PIN, PORTB, D5_PIN, PORTB, D6_PIN, PORTB, D7_PIN);
void message();
uint8_t rightArrow[8] = {
    0x00,  // 10000
    0x18,  // 11000
    0x1C,  // 11100
    0x1E,  // 11110
    0x1F,  // 11111
    0x1E,  // 11110
    0x1C,  // 11100
    0x18,  // 11000
};

int main(void) {
    lcd.init();
    lcd.createChar(0, rightArrow); 
    message();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.displayCustomChar(0);
    lcd.print("Homepage");
    *((volatile uint8_t *)((uint16_t)&PORTB - 1)) |= (1 << 5);

    while (1) {
        // Main loop
        PORTB ^= (1<<5);
        _delay_ms(500);
    }

    return 0;
}
void message(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("--KAWACH--");
    _delay_ms(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Powered by:");
    lcd.setCursor(1,0);
    lcd.print("Kaicho Group");
    _delay_ms(2000);
}