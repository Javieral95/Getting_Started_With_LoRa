"""
 * Configuration file
 * Gateway to reveive arduino node's data using LoRaWAN
 * Tutorial: https://docs.pycom.io/tutorials/networks/lora/lorawan-nano-gateway/
 * Frecuency: 868 MHz for Europe
 * CTIC - LoRa Test
"""

import machine
import ubinascii

#WIFI_MAC = ubinascii.hexlify(machine.unique_id()).upper() #TTN
WIFI_MAC = ubinascii.hexlify(machine.unique_id()) #Chirpstark

# Set  the Gateway ID to be the first 3 bytes of MAC address + 'FFFE' + last 3 bytes of MAC address
#GATEWAY_ID = WIFI_MAC[:6] + "FFFE" + WIFI_MAC[6:12] #TTN
#GATEWAY_ID = WIFI_MAC[:6] + "FFFF" + WIFI_MAC[6:12] #TTN
GATEWAY_ID = WIFI_MAC[:6] + "ffff" + WIFI_MAC[6:12] #Chirpstark
#The Things of Network (V > 3): 'eu1.cloud.thethings.network'
#Chirpstark: 'loraserver.pycom.io' #(O lo que proceda)
SERVER = 'loraserver.pycom.io'

PORT = 1700

NTP = "pool.ntp.org"
NTP_PERIOD_S = 3600

#WiFi
WLAN_SSID = "foo" #"pycom-wifi"
WLAN_PASS = "123abc123" #"securepassword"
WLAN_TIMEOUT_MS = 180000

# for EU868
LORA_FREQUENCY = 868500000
LORA_GW_DR = "SF7BW125" # DR_5
LORA_NODE_DR = 5

# --- Functions ---
def get_gateway_id():
    print("Your gateway_id is: {}".format(GATEWAY_ID)) #The gateway is b'THIS_STRING'
    return GATEWAY_ID
