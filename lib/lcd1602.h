
#ifndef LCD1602_H
#define LCD1602_H
#include <avr/io.h>
#include <util/delay.h>
class LCD1602 {
public:
    // Constructor to initialize the LCD with user-defined pins and ports for control and data pins
    LCD1602(volatile uint8_t &rsPort, uint8_t rsPin,
            volatile uint8_t &enPort, uint8_t enPin,
            volatile uint8_t &dataPort1, uint8_t d4Pin,
            volatile uint8_t &dataPort2, uint8_t d5Pin,
            volatile uint8_t &dataPort3, uint8_t d6Pin,
            volatile uint8_t &dataPort4, uint8_t d7Pin);

    // Function to initialize the LCD
    void init();

    // Function to send a command to the LCD
    void sendCommand(uint8_t command);

    // Function to send data to the LCD
    void sendData(uint8_t data);

    // Function to clear the LCD screen
    void clear();

    // Function to set the cursor to a specific row and column
    void setCursor(uint8_t row, uint8_t col);

    // Function to print a string to the LCD
    void print(const char* str);
    void createChar(uint8_t location, uint8_t* pattern);
    void displayCustomChar(uint8_t location);

private:
    // Function to write a nibble to the data pins
    void writeNibble(uint8_t nibble);

    // Function to set or clear a pin based on the state
    void writePin(volatile uint8_t &port, uint8_t pin, uint8_t state);

    // Member variables for port and pin configurations
    volatile uint8_t &RS_PORT, &EN_PORT;
    volatile uint8_t &D4_PORT, &D5_PORT, &D6_PORT, &D7_PORT;
    uint8_t RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN;
};

#endif // LCD1602_H
