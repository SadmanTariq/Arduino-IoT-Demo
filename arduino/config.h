#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID /* WiFi SSID */
#define WIFI_PASSWORD /* WiFi Password */

#define RTDB_HOST /* Firebase Realtime Database Server Name */
#define RTDB_PORT 443 // 443 for HTTPS

// Uncomment to log to serial.
// #define LOG_TO_SERIAL
#define SERIAL_CONNECTION_TIMEOUT 10000 // Stop trying to connect to serial after 10 seconds

#define SYNC_INTERVAL 5000 // Synchonise with the database every 5 seconds

#endif // CONFIG_H