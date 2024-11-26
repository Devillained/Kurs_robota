#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Ініціалізація LCD-дисплея
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int morseInputPin = 4;   // Кнопка введення коду Морзе
const int buzzerPin = 3;       // П’єзозумер

String morseCode = "";         // Буфер для введеного коду Морзе
String textMessage = "";       // Повідомлення у текстовій формі
unsigned long pressStartTime = 0; // Час початку натискання кнопки
bool buttonPressed = false;    // Стан кнопки введення

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
    lcd.print("Enter Morse:");

    pinMode(morseInputPin, INPUT_PULLUP);
    pinMode(buzzerPin, OUTPUT);
}

void loop() {
    // Обробка кнопки введення коду Морзе
    if (digitalRead(morseInputPin) == LOW) {
        if (!buttonPressed) {
            buttonPressed = true;
            pressStartTime = millis();
            digitalWrite(buzzerPin, HIGH); // Звуковий сигнал
        }
    } else if (buttonPressed) {
        buttonPressed = false;
        digitalWrite(buzzerPin, LOW); // Зупинка звукового сигналу

        unsigned long pressDuration = millis() - pressStartTime;
        if (pressDuration <= 200) {
            morseCode += "."; // Коротке натискання
        } else {
            morseCode += "-"; // Довге натискання
        }

        lcd.setCursor(0, 1);
        lcd.print(morseCode); // Відображення коду Морзе
    }

    // Кодування у текст
    if (morseCode.length() > 0 && digitalRead(morseInputPin) == LOW) {
        String decodedChar = decodeMorse(morseCode);
        if (decodedChar != "") {
            textMessage += decodedChar;
            morseCode = "";
        }

        lcd.setCursor(0, 0);
        lcd.print("Text: ");
        lcd.setCursor(6, 0);
        lcd.print(textMessage); // Відображення тексту
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
