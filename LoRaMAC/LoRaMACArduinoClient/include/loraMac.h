/*
 * Lora's functions using Hardware Abstraction layer
 * CTIC - LoRa MAC Test
 * LoRa Mac functions
 */

/** INCLUDES **/
#include "hal.h"

/** DEFINES **/
#define LORAWAN_FRECUENCY 868E6
#define LORA_SYNC_WORD 0x12



/** FUNCTIONS **/
void initLoraMACTransceiver();
void initLoraMACCommunication();

void onTxDone();
void onReceive(int);
void LoRa_txMode();
void LoRa_rxMode();
void LoRa_sendMessage(String);
