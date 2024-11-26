#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte address[6] = "00001"; // Адреса RF
const int button = 2; // Кнопка підключена до D2
String morseCode = "";

void setup() {
    pinMode(button, INPUT_PULLUP); // Вбудований підтягуючий резистор
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_LOW);
}

void loop() {
    static unsigned long pressTime = 0;

    if (digitalRead(button) == LOW) { // Кнопка натиснута
        if (pressTime == 0) {
            pressTime = millis();
        }
    } else if (pressTime > 0) { // Кнопка відпущена
        unsigned long duration = millis() - pressTime;
        if (duration < 300) { // Коротке натискання (крапка)
            morseCode += ".";
        } else { // Довге натискання (тире)
            morseCode += "-";
        }
        pressTime = 0;
    }

    if (morseCode.length() > 0 && millis() - pressTime > 1000) { // Пауза між символами
        radio.write(&morseCode, sizeof(morseCode));
        morseCode = ""; // Скидаємо код
    }
}
