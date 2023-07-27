#include <SPI.h>
#include <WiFiNINA.h>

#include "config.h"
#include "wifi.h"
#include "firebase.h"
#include "sensors.h"

Wifi wifi;
Firebase firebase;

void setup()
{
    // Initialize serial and wait for port to open:
    Serial.begin(9600);

    int serialTimeout = 10000;
    int lastMillis = millis();
    while (!Serial || serialTimeout <= 0) // wait for serial port to connect. Needed for native USB port only
    {
        serialTimeout -= millis() - lastMillis;
        lastMillis = millis();
    }

    for (int pin = 2; pin <= 15; pin++)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    wifi.initialise();
    initialiseSensors();

    digitalWrite(LED_BUILTIN, HIGH); // turn LED on once everything is ready.
}

void loop()
{
    updateSensors();
    // firebase.setPins();
    firebase.update();
}