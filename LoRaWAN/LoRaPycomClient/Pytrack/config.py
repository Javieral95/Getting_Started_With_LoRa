"""
 * Configuration file
 * Client to send data using LoRaWAN
 * Tutorial: https://docs.pycom.io/tutorials/networks/lora/lorawan-otaa/
 * Frecuency: 868 MHz for Europe (Remember 1% law)
 * CTIC - LoRa Test
"""

import machine
import ubinascii
from network import LoRa

##### INIT LORA OBJECT #####
lora = LoRa(mode=LoRa.LORAWAN, device_class = LoRa.CLASS_A, region=LoRa.EU868)

#### CHIRPSTARK SETTINGS #####
LORA_MAC = ubinascii.hexlify(LoRa().mac()).upper()

CONFIRM_MESSAGES = False #adr

# OTAA
APP_KEY = ubinascii.unhexlify('XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX') # Copy from Chirpstack
APP_EUI = ubinascii.unhexlify('0000000000000000')

DEV_EUI_STR = LORA_MAC[:6] + "FFFE" + LORA_MAC[6:12] # Copy in Chirpstack
DEV_EUI = ubinascii.unhexlify(DEV_EUI_STR) # Use if you dont know the Dev_eui
#DEV_EUI = ubinascii.unhexlify('XXXXXXXXXXXXXXXX') # Use if you have the Dev_eui
######################################

NTP = "es.pool.ntp.org"
NTP_PERIOD_S = 3600

#WiFi settings (change it)
USE_WIFI = False
WLAN_SSID = "foo"
WLAN_PASS = "123abc123"
WLAN_TIMEOUT_MS = 180000

#Spreading Factor: (Higuer value in spreading factor = More distance but less speed transmision)
# Spreading factor range: SF7 (SF7BW250 also exists) to SF15
# To Change Spreading Factor you can Change DataRate in the following line (values 0 to 6, less data rate = more Spreading factor)
LORA_NODE_DR = 5 # 5 (6 using 250Khz) for SF7, 4 for SF6... all using 125Khz
###

# --- Functions ---
def get_dev_eui():
    print("Your dev_eui is: {}".format(DEV_EUI_STR)) #The gateway is b'THIS_STRING'
    return DEV_EUI
