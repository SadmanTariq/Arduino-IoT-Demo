#ifndef FIREBASE_CPP
#define FIREBASE_CPP

#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "firebase.h"
#include "wifi.h"
#include "sensors.h"

WiFiSSLClient getPinsFlagWifiClient;
// HttpClient getPinsFlagClient(getPinsFlagWifiClient, GET_PINS_FLAG_SERVER, 443);

// WiFiSSLClient rtdbWifiClient;
HttpClient rtdbClient(getPinsFlagWifiClient, RTDB_SERVER, 443);


// void Firebase::setPins()
// {
//     // TODO: Error handling
//     Serial.println(F("Getting flag"));
//     getPinsFlagClient.get("/"); // TODO: do something with this output

//     int responseStatusCode = getPinsFlagClient.responseStatusCode();
//     if (responseStatusCode != 200)
//     {
//         Serial.print("Error getting pins flag: ");
//         Serial.println(responseStatusCode);
//         return;
//     }

//     Serial.print("Setting pins to: ");

//     byte pin = 15;
//     getPinsFlagClient.skipResponseHeaders();
//     while (!getPinsFlagClient.endOfBodyReached() && pin >= 2)
//     {
//         char c = getPinsFlagClient.read();
//         Serial.print(c);
//         digitalWrite(pin, c == '1' ? HIGH : LOW);
//         pin--;
//     }
// }

void send()
{
    getPinsFlagWifiClient.println();
    Serial.println();
}

void send(char *s)
{
    getPinsFlagWifiClient.println(s);
    Serial.println(s);
}

void Firebase::sendSensorStatus()
{
    rtdbClient.connectionKeepAlive();
    // char buffer[256];
    // sprintf(buffer, "{\"temperature\":%f,\"pressure\":%d,\"humidity\":%f,\"altitude\":%f,\"gas_resistance\":%d,\"lpg_content\":%d}",
    //         getSensorData()->temperature,
    //         getSensorData()->pressure,
    //         getSensorData()->humidity,
    //         getSensorData()->altitude,
    //         getSensorData()->gasResistance,
    //         getSensorData()->lpgContent);
    char buffer[] = "\"test\": \"test\"";

    // if (getPinsFlagWifiClient.connect(RTDB_SERVER, 443))
    // {
    //     Serial.println("connected to server");
    //     // Make a HTTP request:
    //     send("PATCH /sensors.json HTTP/1.1");
    //     send("Host: " RTDB_SERVER);
    //     send("Content-Type: application/json");
    //     send("Connection: close");
    //     send();
    //     send(buffer);
    //     send();
    // }

    // while (!rtdbClient.available())
    //     yield();

    if (int status = rtdbClient.put("/sensors.json", "application/json", buffer) != 0)
    {
        Serial.print("Error sending sensor status. Error code: ");
        Serial.println(status);
        rtdbClient.stop();
    }

    if (int statusCode = rtdbClient.responseStatusCode() != 200)
    {
        Serial.print("Error sending sensor status. Status code: ");
        Serial.println(statusCode);
        rtdbClient.stop();
    }
}

#define UPDATE_INTERVAL 1000
int updateTimer = UPDATE_INTERVAL;
uint32_t lastMillis = millis();

void Firebase::update()
{
    // while (getPinsFlagWifiClient.available())
    // {
    //     char c = getPinsFlagWifiClient.read();
    //     Serial.write(c);
    // }

    updateTimer -= millis() - lastMillis;
    lastMillis = millis();
    if (updateTimer > 0)
    {
        return;
    }

    // setPins();
    sendSensorStatus();

    updateTimer = UPDATE_INTERVAL;
}

#endif // FIREBASE_CPP