#ifndef CONNECT_h
#define CONNECT_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <BlynkSimpleEsp8266.h>
#include "config.h"

char *ssid;
char *pass;

// Blynk
char auth[] = BLYNK_AUTH;
char server[] = BLYNK_HOST;
uint16_t port = 8442;

uint16_t tries = 0;
uint16_t wifiTries = 0;

void restart();
void reconnectBlynk();
void update();
void connect();

void connect()
{
    tries = 0;
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    uint16_t n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
    {
        Serial.println("no networks found");
    }
    else
    {
        for (uint16_t i = 0; i < n; ++i)
        {
            for (uint16_t n = 0; n < SSID_COUNT; n++)
            {
                if (WiFi.SSID(i) == String(ssids[n].ssid))
                {
                    ssid = ssids[n].ssid;
                    pass = ssids[n].pass;
                }
            }
            delay(10);
        }
    }

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    if (strlen(ssid) > 0)
    {
        Blynk.begin(auth, ssid, pass, server, port);
    }
    else
    {
        Serial.println("no matching ssids found");
    }
    if (!Blynk.connected())
    {
        wifiTries++;
        if (wifiTries >= 3)
        {
            restart();
        }
    }
}
void reconnectBlynk()
{
    if (!Blynk.connected())
    {
        if (Blynk.connect())
        {
            tries = 0;
            BLYNK_LOG("Reconnected");
        }
        else
        {
            tries++;
            BLYNK_LOG("Not reconnected");
            if (tries >= 3)
            {
                connect();
                delay(1000);
            }
        }
    }
}

void update()
{
    Serial.print("[update] Running version: ");
    Serial.println(FIRMWARE_VERSION);
    Serial.println("[update] Checking for update...");
    t_httpUpdate_return ret = ESPhttpUpdate.update(host, httpsPort, url, FIRMWARE_VERSION, true, fingerprint, true);
    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
        Serial.printf("[update] Update failed. Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        Serial.println("");
        break;
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("[update] No update.");
        break;
    case HTTP_UPDATE_OK:
        Serial.println("[update] Update ok."); // may not called we reboot the ESP
        break;
    }
}

void restart()
{
    Blynk.disconnect();
    ESP.restart();
}

#endif
