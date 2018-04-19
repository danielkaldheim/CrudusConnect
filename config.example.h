#ifndef CONFIG_h
#define CONFIG_h
#include <Arduino.h>

#define FIRMWARE_VERSION "project-name-1.0.0"

// Blynk
#define BLYNK_PRINT Serial // Comment this out to disable prints and save space
#define BLYNK_AUTH ""
#define BLYNK_HOST ""

typedef struct
{
    char *ssid;
    char *pass;
} SSID;

#define SSID_COUNT 1
SSID ssids[SSID_COUNT] = {
    {"your_ssid", "your_ssid_password"},
};

// Over the air update
const char *host = "example.com";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char *fingerprint = "";
const char *url = "path/to/arduino.php";

#endif