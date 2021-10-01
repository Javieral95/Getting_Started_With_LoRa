/*
 * Hardware Abstraction Layer (HAL)
 * CTIC - LoRa MAC Test
 */

#include "hal.h"

void initSerialMonitor(){
    Serial.begin(MONITOR_SPEED);
}

void initTempSensor()
{
    //TO-DO: Init AM2315 sensor
}
