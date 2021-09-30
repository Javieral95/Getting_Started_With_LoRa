"""
 * Main file
 * Gateway to reveive arduino node's data using LoRaWAN
 * Tutorial: https://docs.pycom.io/tutorials/networks/lora/lorawan-nano-gateway/
 * Frecuency: 868 MHz for Europe
 * CTIC - LoRa Test
"""

import sys
import gc
import socket
import time

import machine
import pycom
import utime

import urequests
import ubinascii

from network import WLAN, LoRa

from pycoproc_1 import Pycoproc
from LIS2HH12 import LIS2HH12 #Acelerometro
from L76GNSS import L76GNSS #GPS

import config
from nanogateway import NanoGateway

#=======================
#===== CONSTANTS =======
#=======================
ENABLE_WIFI = True

LOOP_INTERVAL = 0.6


#=======================
#====== AUXILIAR =======
#=======================

#Garbage collector & separator
def enable_gc():
    print("Enabling garbage collector")
    time.sleep(2.0)
    gc.enable()
    print("Garbage collector: OK")

def print_separator():
    print("\n------\n")

#Network
def setup_rtc(rtc):
    print("RTC NTP sync")
    rtc.ntp_sync("pool.ntp.org")
    utime.sleep_ms(750)
    print("RTC set from NTP to UTC: {}".format(rtc.now()))
    utime.timezone(7200)
    print('Adjusted from UTC to Madrid timezone', utime.localtime(), '\n')

def connect_wlan(wlan):
    print("Connecting to SSID: {}".format(config.WLAN_SSID))

    print("Try to connect . . .")
    wlan.connect(
        ssid=config.WLAN_SSID,
        auth=(WLAN.WPA2,config. WLAN_PASS),
        timeout=config.WLAN_TIMEOUT_MS)
    print("Connecting . . .")
    now = time.ticks_ms()

    while not wlan.isconnected():
        if (time.ticks_ms() - now) > config.WLAN_TIMEOUT_MS:
            raise TimeoutError("WiFi timeout")

        print("Still disconnected")
        pycom.rgbled(0xFF0000)
        time.sleep(0.5)
        pycom.rgbled(0x000000)
        time.sleep(0.5)

    pycom.rgbled(0x000000)
    print("WiFi connected succesfully")
    print("wlan.ifconfig:\n{}".format(wlan.ifconfig()))

def init_loraWAN_gateway():
    print("Initializing LoRaWAN nano Gateway")
    nanogw = NanoGateway(
        id=config.GATEWAY_ID,
        frequency=config.LORA_FREQUENCY,
        datarate=config.LORA_GW_DR,
        ssid=config.WLAN_SSID,
        password=config.WLAN_PASS,
        server=config.SERVER,
        port=config.PORT,
        ntp_server=config.NTP,
        ntp_period=config.NTP_PERIOD_S
        )
    print("Ok! Now you have a LoRaWAN Gateway!")
    nanogw.start()
    nanogw._log('You may now press ENTER to enter the REPL')
    input()

#Data
def post_data_to_db(acceleration, pitch, roll, coord):
    try:
        url = "{}?{}".format(INFLUX_WRITE_URL, INFLUX_WRITE_ARGS)
        device = ubinascii.hexlify(machine.unique_id()).decode()
        data = "device={} acceleration={} pitch={} roll={} coord={}".format(
            device, acceleration, pitch, roll, coord)

        print("Uploading: {}".format(data))

        res = urequests.post(
            url,
            headers={
                "Content-Type": "text/plain",
                "Authorization": "Token {}".format(INFLUX_TOKEN)
            },
            data=str(data))

        res.close()
    except Exception as ex:
        print("HTTP error: {}".format(ex))

#Others (It isnt important for LoRa)
def wait_next_loop():
    time.sleep(LOOP_INTERVAL / 2)
    pycom.rgbled(0x000000)
    time.sleep(LOOP_INTERVAL / 2)


#=======================
#======== MAIN =========
#=======================
def main():
    pycom.heartbeat(False)
    enable_gc();
    pycoproc = Pycoproc(Pycoproc.PYTRACK)

    config.get_gateway_id()

    #if ENABLE_WIFI:
        #wlan = WLAN(mode=WLAN.STA)
        #connect_wlan(wlan=wlan)

    init_loraWAN_gateway()

    #while True:
    #    wait_next_loop()
    #    print_separator()

#END MAIN & CALL
main();
