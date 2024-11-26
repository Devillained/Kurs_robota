#include <SPI.h>
#include <RF24.h>

// Ініціалізація модуля NRF24L01
RF24 radio(9, 10);                   // CE - 9, CSN - 10
const byte address[6] = "PRX";       // Адреса приймача

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(address);  // Відкриття каналу для передачі
    radio.setPALevel(RF24_PA_LOW);   // Встановлення потужності сигналу
    radio.stopListening();           // Перехід у режим передачі
}

void loop() {
    String message = "Hello NRF24L01"; // Повідомлення для передачі
    Serial.println("Sending: " + message);

    // Відправлення повідомлення
    bool success = radio.write(message.c_str(), message.length() + 1);

    if (success) {
        Serial.println("Message sent successfully!");
    } else {
        Serial.println("Message failed to send.");
    }

    delay(2000); // Затримка між передачами
}
