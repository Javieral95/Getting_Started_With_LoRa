/*
 * Main file to read config and run the loop.
 * Run a Arduino Node for LoRaWAN using OTAA and a service (Chipstark)
 * Configure pinmap in hal.h and Lora config in loraWan.h
 * CTIC - LoRa Test
 */

#include <loraWan.h>
int INTERVAL_TIME = 60000; //ms

String data = "Hello World";

void setup()
{
    initSerialMonitor();
    //Serial.begin(MONITOR_SPEED);
    Serial.println(F("Starting"));

    //lora Config
    init_lora();
    connect_lora_otaa();
    config_lora();
}

void loop()
{
    //Serial.println(F("Starting"));
    send_data(data, WAIT_FOR_RESPONSE);
    delay(INTERVAL_TIME);
}