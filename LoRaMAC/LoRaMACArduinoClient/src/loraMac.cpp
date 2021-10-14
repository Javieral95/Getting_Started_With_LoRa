/*
 * Lora's functions using Hardware Abstraction layer
 * CTIC - LoRa MAC Test
 * LoRa Mac functions
 */

#include "loraMac.h"

/* Init Functions */
void initLoraMACTransceiver()
{
    Serial.println("Setting LoRa MAC Pins . . .");
    LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);
}

void initLoraMACCommunication()
{
    Serial.println("Try to init Lora:");
    while (!LoRa.begin(LORAWAN_FRECUENCY))
    {
        Serial.print(". ");
        delay(500);
    }
    LoRa.setSyncWord(LORA_SYNC_WORD); // The sync word assures you don't get LoRa messages from other LoRa transceivers ranges from 0-0xFF
    Serial.println("LoRa connect succesfully!");
}

/* Functions */
void LoRa_rxMode()
{
    LoRa.enableInvertIQ(); // active invert I and Q signals
    LoRa.receive();        // set receive mode
}

void LoRa_txMode()
{
    LoRa.idle();            // set standby mode
    LoRa.disableInvertIQ(); // normal mode
}

void LoRa_sendMessage(String message)
{
    Serial.print("Sending: {");
    Serial.print(message);
    Serial.println("}");

    LoRa_txMode();        // set tx mode
    LoRa.beginPacket();   // start packet
    LoRa.print(message);  // add payload
    LoRa.endPacket(true); // finish packet and send it
    printLoraInfoInDisplay();
}

void onReceive(int packetSize)
{
    String message = "";

    while (LoRa.available())
    {
        message += (char)LoRa.read();
    }

    Serial.print("Node Receive: ");
    Serial.println(message);
}

void onTxDone()
{
    Serial.println("TxDone");
    LoRa_rxMode();
}