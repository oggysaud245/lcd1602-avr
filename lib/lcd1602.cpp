#include "lcd1602.h"

// Constructor to initialize the LCD with user-defined pins and ports for control and data pins
LCD1602::LCD1602(volatile uint8_t &rsPort, uint8_t rsPin,
                 volatile uint8_t &enPort, uint8_t enPin,
                 volatile uint8_t &dataPort1, uint8_t d4Pin,
                 volatile uint8_t &dataPort2, uint8_t d5Pin,
                 volatile uint8_t &dataPort3, uint8_t d6Pin,
                 volatile uint8_t &dataPort4, uint8_t d7Pin)
    : RS_PORT(rsPort), EN_PORT(enPort),
    D4_PORT(dataPort1), D5_PORT(dataPort2),
    D6_PORT(dataPort3), D7_PORT(dataPort4),  // Initialize volatile references first
    RS_PIN(rsPin), EN_PIN(enPin),
    D4_PIN(d4Pin), D5_PIN(d5Pin),
    D6_PIN(d6Pin), D7_PIN(d7Pin) {} 
// Function to initialize the LCD
void LCD1602::init() {
    // Set DDRX control pins as output
    *((volatile uint8_t *)((uint16_t)&RS_PORT - 1)) |= (1 << RS_PIN); 
    *((volatile uint8_t *)((uint16_t)&EN_PORT - 1)) |= (1 << EN_PIN);  // Set EN_PIN as output
    *((volatile uint8_t *)((uint16_t)&D4_PORT - 1)) |= (1 << D4_PIN);  // Set D4_PIN as output
    *((volatile uint8_t *)((uint16_t)&D5_PORT - 1)) |= (1 << D5_PIN);  // Set D5_PIN as output
    *((volatile uint8_t *)((uint16_t)&D6_PORT - 1)) |= (1 << D6_PIN);  // Set D6_PIN as output
    *((volatile uint8_t *)((uint16_t)&D7_PORT - 1)) |= (1 << D7_PIN);  // Set D7_PIN as output
    

    _delay_ms(100);  // Wait for more than 100 ms after VCC rises to 4.5V

    // Initialize in 4-bit mode (command to function set)
    sendCommand(0x02);  // Send 4-bit mode initialization
    sendCommand(0x28);  // Function set: 4-bit, 2-line, 5x8 font
    sendCommand(0x0C);  // Display ON, Cursor OFF
    sendCommand(0x06);  // Entry mode: Increment cursor
    sendCommand(0x01);  // Clear display
    _delay_ms(20);      // Wait for clear to finish
}

// Function to send a command to the LCD
void LCD1602::sendCommand(uint8_t command) {
    RS_PORT &= ~(1 << RS_PIN);  // RS = 0 for command
    EN_PORT &= ~(1 << EN_PIN);  // EN = 0 (to start fresh)

    // Send upper nibble (4 bits)
    writeNibble(command >> 4);  // Mask and shift upper nibble
    EN_PORT |= (1 << EN_PIN);  // EN = 1 to start operation
    _delay_us(20);
    EN_PORT &= ~(1 << EN_PIN);  // EN = 0 to end operation
    _delay_us(20);

    // Send lower nibble (4 bits)
    writeNibble(command & 0x0F);  // Mask and shift lower nibble
    EN_PORT |= (1 << EN_PIN);  // EN = 1 to start operation
    _delay_us(20);
    EN_PORT &= ~(1 << EN_PIN);  // EN = 0 to end operation
    _delay_us(20);
}

// Function to send data to the LCD
void LCD1602::sendData(uint8_t data) {
    RS_PORT |= (1 << RS_PIN);  // RS = 1 for data
    EN_PORT &= ~(1 << EN_PIN);  // EN = 0 (to start fresh)

    // Send upper nibble (4 bits)
    writeNibble(data >> 4);  // Mask and shift upper nibble
    EN_PORT |= (1 << EN_PIN);  // EN = 1 to start operation
    _delay_us(20);
    EN_PORT &= ~(1 << EN_PIN);  // EN = 0 to end operation
    _delay_us(20);

    // Send lower nibble (4 bits)
    writeNibble(data & 0x0F);  // Mask and shift lower nibble
    EN_PORT |= (1 << EN_PIN);  // EN = 1 to start operation
    _delay_us(20); 
    EN_PORT &= ~(1 << EN_PIN);  // EN = 0 to end operation
    _delay_us(20); 
}

// Function to clear the LCD screen
void LCD1602::clear() {
    sendCommand(0x01);  // Clear display command
    _delay_ms(10);       // Wait for clear to finish
}

// Function to set the cursor to a specific row and column
void LCD1602::setCursor(uint8_t row, uint8_t col) {
    uint8_t address = 0;

    if (row == 0)
        address = 0x80 + col;  // First row
    else if (row == 1)
        address = 0xC0 + col;  // Second row

    sendCommand(address);
}

// Function to print a string to the LCD
void LCD1602::print(const char* str) {
    while (*str) {
        sendData(*str++);
    }
}

// Function to write a nibble to the data pins
void LCD1602::writeNibble(uint8_t nibble) {
    // Write each pin (D4 to D7) with the corresponding bits of the nibble
    writePin(D4_PORT, D4_PIN, (nibble & 0x01));
    writePin(D5_PORT, D5_PIN, (nibble >> 1) & 0x01);
    writePin(D6_PORT, D6_PIN, (nibble >> 2) & 0x01);
    writePin(D7_PORT, D7_PIN, (nibble >> 3) & 0x01);
}

// Function to set or clear a pin based on the state
void LCD1602::writePin(volatile uint8_t &port, uint8_t pin, uint8_t state) {
    if (state)
        port |= (1 << pin);  // Set pin high
    else
        port &= ~(1 << pin);  // Set pin low
}
void LCD1602::createChar(uint8_t location, uint8_t* pattern) {
    if (location < 8) {  // We can store 8 custom characters at max (0x00 to 0x07)
        sendCommand(0x40 + (location * 8));  // Set CGRAM address (0x40 to 0x47)
        
        // Write the 8 bytes of the custom character to CGRAM
        for (uint8_t i = 0; i < 8; i++) {
            sendData(pattern[i]);  // Send each row of the custom character
        }
    }
}
void LCD1602::displayCustomChar(uint8_t location) {
    // Display the custom character by referring to its CGRAM location (index)
    sendData(location);  // Send the index of the custom character to the LCD
}