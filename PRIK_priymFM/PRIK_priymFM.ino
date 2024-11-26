#include <SPI.h>
#include <RF24.h>

// Ініціалізація модуля NRF24L01
RF24 radio(9, 10);                   // CE - 9, CSN - 10
const byte address[6] = "PRX";       // Адреса приймача

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(1, address);  // Відкриття каналу для прийому
    radio.setPALevel(RF24_PA_LOW);      // Встановлення потужності сигналу
    radio.startListening();             // Перехід у режим прийому
    Serial.println("Ready to receive data...");
}

void loop() {
    if (radio.available()) {
        char buffer[32] = "";          // Буфер для збереження отриманих даних
        radio.read(&buffer, sizeof(buffer));  // Зчитування даних у буфер
        Serial.print("Received: ");
        Serial.println(buffer);        // Вивід отриманого повідомлення
    }
}
