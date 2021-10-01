/*
 * Hardware Abstraction Layer (HAL)
 * CTIC - LoRa Test
 */

#include "hal.h"

//Objects
//OneWire oneWireSensor(SENSOR_PIN);
//DallasTemperature tempSensor(&oneWireSensor);
/*
void initLoraTransceiver(){
    Serial.println("Setting LoRa Pins . . .");
    LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);
}

void initLoraCommunication(){
    Serial.println("Try to connect LoraWAN:");
    while(!LoRa.begin(LORAWAN_FRECUENCY)){
        Serial.print(". ");
        delay(500);
    }
    //LoRa.setSyncWord(LORA_SYNC_WORD); // The sync word assures you don't get LoRa messages from other LoRa transceivers ranges from 0-0xFF
    Serial.println("LoRa connect succesfully!");
}

void initSerialMonitor(){
    Serial.begin(MONITOR_SPEED);
}

void initTempSensor()
{
    
}
*/