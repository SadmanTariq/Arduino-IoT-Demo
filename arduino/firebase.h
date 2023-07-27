#ifndef FIREBASE_H
#define FIREBASE_H

#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>

#define GET_PINS_FLAG_SERVER "getpinsflag-hok5b3oika-uc.a.run.app"
#define RTDB_SERVER "arduino-iot-demo-785d8-default-rtdb.asia-southeast1.firebasedatabase.app"


class Firebase
{
public:
    void setPins();
    void sendSensorStatus();
    void update();
};

#endif // FIREBASE_H