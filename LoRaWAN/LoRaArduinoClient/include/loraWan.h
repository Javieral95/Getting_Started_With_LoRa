/*
 * Lora's functions using Hardware Abstraction layer
 * CTIC - LoRa WAN Test
 * LoRa WAN functions
 */

#include "hal.h"

#define LORAWAN_FRECUENCY 868E6
#define LORA_SYNC_WORD 0xF3

void initLoraWANTransceiver();
void initLoraWANCommunication();


void onEvent();
void do_send(osjob_t *);
//void LoRa_txMode();
//void LoRa_rxMode();
//void LoRa_sendMessage(String);
