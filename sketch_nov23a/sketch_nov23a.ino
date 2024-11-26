#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Змініть 0x27 на знайдену адресу

void setup() {
  lcd.init();          // Ініціалізація дисплея
  lcd.backlight();     // Увімкнення підсвітки
  lcd.setCursor(0, 0); // Встановлення курсора
  lcd.print("Hello, World!"); // Текст у першому рядку
}

void loop() {}
