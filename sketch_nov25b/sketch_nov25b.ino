#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// RF24 and I2C LCD Initialization
RF24 radio(9, 10);                   // CE - 9, CSN - 10
const byte address[6] = "PRX";       // Address for the receiver
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C LCD

// Device Name
const String deviceName = "PRX";

String receivedMessage = ""; // Buffer for received message
unsigned long lastScrollTime = 0;    // For text scrolling
int scrollOffset = 0;                // Current scroll position

void setup() {
    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Frm: PER"); // Display connected transmitter name

    // Initialize RF24
    radio.begin();
    radio.openReadingPipe(1, address);
    radio.setPALevel(RF24_PA_LOW);
    radio.startListening();
}

void loop() {
    // Buffer for RF24 data
    char buffer[32] = "";

    // Check for incoming messages
    if (radio.available()) {
        // Read the message into the buffer
        radio.read(&buffer, sizeof(buffer));
        
        // Validate received data
        bool isValid = false;
        for (int i = 0; i < sizeof(buffer); i++) {
            if (isPrintable(buffer[i])) { // Check if the character is printable
                isValid = true;
                break;
            }
        }

        if (isValid) {
            receivedMessage = String(buffer); // Convert buffer to String

            // Reset scrolling when new message is received
            scrollOffset = 0;

            // Display receiving status
            lcd.setCursor(0, 0);
            lcd.print("Frm: PER      ");
        }
    }

    // Handle message scrolling
    if (receivedMessage.length() > 16) { // If the message exceeds the display width
        if (millis() - lastScrollTime > 500) { // Scroll every 500ms
            scrollOffset++;
            if (scrollOffset > receivedMessage.length() - 16) {
                scrollOffset = receivedMessage.length() - 16; // Stop scrolling at the end
            }
            lastScrollTime = millis();
        }
    } else {
        scrollOffset = 0; // Reset scroll if message fits
    }

    // Display received message on the LCD
    lcd.setCursor(0, 1);
    lcd.print("                "); // Clear the row completely
    lcd.setCursor(0, 1);
    String displayText = receivedMessage.substring(scrollOffset, scrollOffset + 16);
    lcd.print(displayText);
}
