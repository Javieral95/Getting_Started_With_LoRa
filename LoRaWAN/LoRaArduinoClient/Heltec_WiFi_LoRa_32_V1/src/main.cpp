/*
 * Main file to read config and run the loop.
 * Run a Arduino Node for LoRaWAN using OTAA and a service (TTS or Chipstark)
 * Configure pinmap in hal.h and Lora config in loraWan.h
 * CTIC - LoRa Test
 */

#include "loraWan.h"

void setup()
{
    initSerialMonitor();
    Serial.println(F("Starting"));

    //AM2315 sensor and display screen init
    initAM2315();
    initDisplay();

    // Start job (sending automatically, starts OTAA too)
    LoraWan_startJob();
}

void loop()
{
    os_runloop_once();
}