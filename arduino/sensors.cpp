#ifndef SENSORS_CPP
#define SENSORS_CPP

#include "sensors.h"
#include <Adafruit_BME680.h>

#define LPG_PIN A6
#define SEALEVELPRESSURE_HPA (1013.25)

SensorData sensorData;
Adafruit_BME680 bme;

void initialiseSensors()
{
    bme.begin(0x76);

    if (!bme.begin(0x76))
    {
        Serial.println("Could not find a valid BME680 sensor, check wiring!");
    }

    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
    bme.beginReading();

    delay(200); // Wait for gas heating to finish before proceeding to prevent overcurrent.
}

void updateSensors()
{
    // Continuously take readings from the sensor.
    if (bme.remainingReadingMillis() <= 0)
    {
        bme.beginReading();
    }
    sensorData.temperature = bme.temperature;
    sensorData.pressure = bme.pressure;
    sensorData.humidity = bme.humidity;
    sensorData.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    sensorData.gasResistance = bme.gas_resistance;
    sensorData.lpgContent = analogRead(LPG_PIN);
}

SensorData *getSensorData()
{
    return &sensorData;
}

#endif // SENSORS_CPP