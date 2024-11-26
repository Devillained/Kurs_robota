#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// RF24 and I2C LCD Initialization
RF24 radio(9, 10);                   // CE - 9, CSN - 10
const byte address[6] = "PRX";       // Address for the receiver
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C LCD

// Device Name
const String deviceName = "PER";

String message = "";                 // Current input message
unsigned long lastScrollTime = 0;    // For text scrolling
int scrollOffset = 0;                // Current scroll position

void setup() {
    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Cto: PRX"); // Display connected receiver name

    // Initialize RF24
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_LOW);
    radio.stopListening();

    // Seed the random number generator
    randomSeed(analogRead(0)); // Use noise on an analog pin for randomness
}

void loop() {
    // Generate a random character if the message length is less than 32
    if (message.length() < 32) {
        char randomChar;
        int randType = random(0, 2); // 0 for letters, 1 for digits

        if (randType == 0) {
            randomChar = char(random(65, 91)); // Generate a random uppercase letter (A-Z)
        } else {
            randomChar = char(random(48, 58)); // Generate a random digit (0-9)
        }

        message += randomChar; // Add the random character to the message
        delay(300); // Simulate typing delay
    }

    // Handle message scrolling
    if (message.length() > 16) { // If the message exceeds the display width
        if (millis() - lastScrollTime > 500) { // Scroll every 500ms
            scrollOffset++;
            if (scrollOffset > message.length() - 16) {
                scrollOffset = message.length() - 16; // Stop scrolling at the end
            }
            lastScrollTime = millis();
        }
    } else {
        scrollOffset = 0; // Reset scroll if message fits
    }

    // Display message on the LCD
    lcd.setCursor(0, 1);
    String displayText = message.substring(scrollOffset, scrollOffset + 16);
    lcd.print("                "); // Clear the row
    lcd.setCursor(0, 1);
    lcd.print(displayText);

    // Example of sending the message
    if (message.length() >= 32) { // Simulate sending message when input is "full"
        radio.write(message.c_str(), message.length() + 1);

        // Display sending status
        lcd.setCursor(0, 0);
        lcd.print("Sent to PRX   ");
        delay(2000); // Wait to simulate sending delay

        // Reset the message after sending
        message = "";
        scrollOffset = 0;
        lcd.setCursor(0, 0);
        lcd.print("Cto: PRX      "); // Restore connected status
    }
}
