"""
 * Configuration file
 * Gateway to reveive arduino node's data using LoRaWAN
 * Tutorial: https://docs.pycom.io/tutorials/networks/lora/lorawan-nano-gateway/
 * Frecuency: 868 MHz for Europe (Remember 1% law)
 * CTIC - LoRa Test
"""

import machine
import ubinascii

#### TTS OR CHIRPSTARK SETTINGS #####
# Set  the Gateway ID to be the first 3 bytes of MAC address + 'FFFF' + last 3 bytes of MAC address

## FOR THE THINGS NETWORK (TTS) ##
#WIFI_MAC = ubinascii.hexlify(machine.unique_id()).upper()
#SERVER = 'eu1.cloud.thethings.network'
#GATEWAY_ID = WIFI_MAC[:6] + "FFFF" + WIFI_MAC[6:12]

## FOR CHIRPSTARK ##
WIFI_MAC = ubinascii.hexlify(machine.unique_id())
SERVER = '192.168.10.140'#'loraserver.pycom.io' #(or url of your server)
GATEWAY_ID = WIFI_MAC[:6] + "ffff" + WIFI_MAC[6:12]
######################################

PORT = 1700

NTP = "es.pool.ntp.org"
NTP_PERIOD_S = 3600

#WiFi settings (change it)
WLAN_SSID = "foo"
WLAN_PASS = "123abc123"
WLAN_TIMEOUT_MS = 180000

### LoRaWAN for EU868 ###
LORA_FREQUENCY = 868500000
#Spreading Factor: (Higuer value in spreading factor = More distance but less speed transmision)
LORA_GW_DR = "SF7BW125" # Can change in range: SF7 (SF7BW250 also exists) to SF15
LORA_NODE_DR = 5 # 5 (6 using 250Khz) for SF7, 4 for SF6... all using 125Khz
###

# --- Functions ---
def get_gateway_id():
    print("Your gateway_id is: {}".format(GATEWAY_ID)) #The gateway is b'THIS_STRING'
    return GATEWAY_ID
