#pragma once 

//by GM 
//GITHUB https://github.com/GavinsMJ

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#if defined ESP8266
#include <ESP8266WiFi.h>
#elif defined ESP32
#include <WiFi.h>
#include <esp_task_wdt.h>
#else
#error Wrong platform Port the code for platform change 
#endif

//3 seconds WDT
#define WDT_TIMEOUT 3

#if DEBUG_Wifi_multi == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif



//Higher Priority networks
const char* PKNOWN_SSID[] = {"RSSIIoT"};
const char* PKNOWN_PASSWORD[] = {"/88/88/88/rss"};

//Lower Priority Networks
const char* KNOWN_SSID[] = {"Gavin__", "Gavins_", "Gavin"};
const char* KNOWN_PASSWORD[] = {"12348765", "qwerty1234",  "/88/88/88/"};

// number of known networks
const int   PKNOWN_SSID_COUNT = sizeof(PKNOWN_SSID) / sizeof(PKNOWN_SSID[0]); 
const int   KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]); 

#include "WiFiConnect.h"
