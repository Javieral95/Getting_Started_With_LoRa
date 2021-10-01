/*
 * Lora's functions using Hardware Abstraction layer
 * CTIC - LoRa MAC Test
 * LoRa Mac functions
 */

#include "hal.h"

#define LORAWAN_FRECUENCY 868E6
#define LORA_SYNC_WORD 0xF3

void initLoraMACTransceiver();
void initLoraMACCommunication();

void onTxDone();
void onReceive(int);
void LoRa_txMode();
void LoRa_rxMode();
void LoRa_sendMessage(String);
