#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "config.h"

#include "sensors.h"

uint8_t wifiStatus = WL_IDLE_STATUS;

const char *ssid = WIFI_SSID;
const char *pass = WIFI_PASSWORD;

WiFiSSLClient wifiClient;
HttpClient rtdbClient = HttpClient(wifiClient, RTDB_HOST, RTDB_PORT);

DynamicJsonDocument jsonDoc(1024);

void setup()
{
#ifdef LOG_TO_SERIAL
    Serial.begin(115200);

    uint32_t startMillis = millis();
    while (!Serial && millis() - startMillis < SERIAL_CONNECTION_TIMEOUT) // wait for serial port to connect.
        yield();
#endif

    initialiseWifi();
    initialiseSensors();
    initialisePins();

    digitalWrite(LED_BUILTIN, HIGH);
}

uint32_t lastSyncMillis = 0;
void loop()
{
    updateSensors();

    if (millis() - lastSyncMillis > SYNC_INTERVAL)
    {
        lastSyncMillis = millis();
        syncDb();
    }
}

void getPins()
{
    rtdbClient.get("/pins.json");

    int responseStatusCode = rtdbClient.responseStatusCode();
    String responseBody = rtdbClient.responseBody();

    Serial.println(responseStatusCode);
    Serial.println(responseBody);

    jsonDoc.clear();
    deserializeJson(jsonDoc, responseBody);

    for (uint8_t pin = 2; pin <= 12; pin++)
    {

        // if (jsonDoc.containsKey(pin))
        // {
        pinMode(pin, OUTPUT);
        bool pinState = jsonDoc[pin];
        digitalWrite(pin, pinState);
        // }
    }
    jsonDoc.clear();
}

void putSensors()
{
    char buffer[256];
    SensorData *sensorData = getSensorData();
    sprintf(buffer, "{"
                    "\"temperature\":%f,"
                    "\"pressure\":%d,"
                    "\"humidity\":%f,"
                    "\"altitude\":%f,"
                    "\"gas_resistance\":%d,"
                    "\"lpg_content\":%d,"
                    "\"potentiometer\":%d"
                    "}",
            sensorData->temperature,
            sensorData->pressure,
            sensorData->humidity,
            sensorData->altitude,
            sensorData->gasResistance,
            sensorData->lpgContent,
            sensorData->potentiometer);

    rtdbClient.put("/sensors.json", "application/json", buffer);

    Serial.println(rtdbClient.responseStatusCode());
    Serial.println(rtdbClient.responseBody());
}

void syncDb()
{
    getPins();
    putSensors();
}

void initialisePins()
{
    for (uint8_t pin = 2; pin <= 13; pin++)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}

void initialiseWifi()
{
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("Communication with WiFi module failed!");
        while (true)
            yield();
    }

    // attempt to connect to Wifi network:
    while (wifiStatus != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);

        wifiStatus = WiFi.begin(ssid, pass);

        // wait for connection:
        delay(5000);
    }

    Serial.println("Connected to wifi");

    printWifiStatus();
}

void printWifiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}