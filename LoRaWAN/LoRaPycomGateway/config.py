"""
 * Configuration file
 * Gateway to reveive arduino node's data using LoRaWAN
 * Tutorial: https://docs.pycom.io/tutorials/networks/lora/lorawan-nano-gateway/
 * Frecuency: 868 MHz for Europe
 * CTIC - LoRa Test
"""

import machine
import ubinascii

#### TTS OR CHIRPSTARK SETTINGS #####
# Set  the Gateway ID to be the first 3 bytes of MAC address + 'FFFE' + last 3 bytes of MAC address

## FOR THE THINGS NETWORK (TTS) ##
#WIFI_MAC = ubinascii.hexlify(machine.unique_id()).upper()
#SERVER = 'eu1.cloud.thethings.network'
#GATEWAY_ID = WIFI_MAC[:6] + "FFFF" + WIFI_MAC[6:12]

## FOR CHIRPSTARK ##
WIFI_MAC = ubinascii.hexlify(machine.unique_id())
SERVER = 'loraserver.pycom.io' #(or url of your server)
GATEWAY_ID = WIFI_MAC[:6] + "ffff" + WIFI_MAC[6:12]
######################################

PORT = 1700

NTP = "pool.ntp.org"
NTP_PERIOD_S = 3600

#WiFi settings (change it)
WLAN_SSID = "foo" #"pycom-wifi"
WLAN_PASS = "123abc123" #"securepassword"
WLAN_TIMEOUT_MS = 180000

# for EU868
LORA_FREQUENCY = 868500000
LORA_GW_DR = "SF7BW125" # DR_5,Can change in range: SF7 to SF15
LORA_NODE_DR = 5

# --- Functions ---
def get_gateway_id():
    print("Your gateway_id is: {}".format(GATEWAY_ID)) #The gateway is b'THIS_STRING'
    return GATEWAY_ID
