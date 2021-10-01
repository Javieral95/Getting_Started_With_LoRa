/*
 * Client to send data with LoRaMAC
 * Using: AM2315 sensor
 * Tutorial: https://www.youtube.com/watch?v=OfH80Gmgf-o
 * Can see: https://github.com/vpcola/ESP32SingleChannelGateway/
 * Frecuency: 868 MHz for Europe
 * CTIC - LoRa MAC Test
 */
#include "hal.h"
#include "loraMAC.h"

const uint INTERVAL_TIME = 1000;
const String TEMP_STRING = "Temp";
const String HUMIDITY_STRING = "Hum";

//Auxiliar Functions
String initLoraMsg(float temp, float hum)
{
  String ret = String("") + temp + "," + hum;
  return ret;
}

//Setup and Loop
void setup()
{
  //Init Serial
  initSerialMonitor();
  while (!Serial)
    ;

  //Init Lora
  initLoraMACTransceiver();
  initLoraMACCommunication();
}

void loop()
{
  // put your main code here, to run repeatedly:
  //To-Do: Read Temp and Hum
  String msg = initLoraMsg(24, 55);
  LoRa_sendMessage(msg);

  delay(INTERVAL_TIME);
}
