#ifndef WIFI_H
#define WIFI_H

#include "config.h"
#include <WiFiNINA.h>

class Wifi
{
    public:
        u_int8_t status = WL_IDLE_STATUS;
        // WiFiSSLClient client;

        // Wifi();
        void initialise();
        void printStatus();

    private:
        char *ssid = WIFI_SSID;
        char *pass = WIFI_PASSWORD;
};


#endif // WIFI_H