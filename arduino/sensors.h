#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

struct SensorData
{
    /// @brief Ambient temperature in degrees Celsius.
    float temperature;

    /// @brief Barometric pressure in Pascals.
    uint32_t pressure;

    /// @brief Relative humidity as a percentage.
    float humidity;

    /// @brief Altitude above sea level in metres.
    float altitude;

    /// @brief Gas resistance in Ohms.
    uint32_t gasResistance;

    /// @brief LPG content, 0 to 1023.
    uint16_t lpgContent;
};


void initialiseSensors();
void updateSensors();
SensorData *getSensorData();


#endif // SENSORS_H