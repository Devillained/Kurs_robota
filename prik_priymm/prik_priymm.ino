#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Ініціалізація LCD-дисплея
LiquidCrystal_I2C lcd(0x27, 16, 2);

String receivedMorseCode = ""; // Отриманий код Морзе
String decodedMessage = "";    // Декодоване повідомлення

// Таблиця коду Морзе
const String morseDict[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
                            "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
                            "..-", "...-", ".--", "-..-", "-.--", "--.."};
const char alphabet[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                         'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                         'U', 'V', 'W', 'X', 'Y', 'Z'};

void setup() {
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Waiting...");
}

void loop() {
    // Симуляція отримання коду Морзе
    if (Serial.available()) {
        char incomingChar = Serial.read();
        if (incomingChar == ' ') { // Пробіл як роздільник
            String decodedChar = decodeMorse(receivedMorseCode);
            if (decodedChar != "") {
                decodedMessage += decodedChar;
            }
            receivedMorseCode = ""; // Скидання буфера коду Морзе
        } else {
            receivedMorseCode += incomingChar;
        }

        lcd.setCursor(0, 0);
        lcd.print("Text: ");
        lcd.setCursor(6, 0);
        lcd.print(decodedMessage); // Відображення тексту
        lcd.setCursor(0, 1);
        lcd.print("Morse: ");
        lcd.setCursor(7, 1);
        lcd.print(receivedMorseCode); // Відображення поточного коду Морзе
    }
}

// Функція декодування коду Морзе
String decodeMorse(String morse) {
    for (int i = 0; i < 26; i++) {
        if (morse == morseDict[i]) {
            return String(alphabet[i]);
        }
    }
    return "";
}
