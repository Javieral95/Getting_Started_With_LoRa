/*
 * Lora's functions using Hardware Abstraction layer
 * CTIC - LoRa WAN Test
 * LoRa WAN functions
 */

/* INCLUDES */
#include "hal.h"
#include <MKRWAN.h>

/* DEFINES */
//LoRA Secrets
#define APP_EUI "0000000000000000"
#define APP_KEY "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" //Copy from Chirpstack
//Config
#define WAIT_FOR_RESPONSE false

/* FUNCTIONS */
void init_lora();
void connect_lora_otaa();
void config_lora();
void send_data(String, bool);
void receive_response();