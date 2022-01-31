import sys
import gc
import socket
import time

import machine
import pycom
import utime

import ubinascii

from network import LoRa
from network import WLAN

##---Pycom Libraries---
from pycoproc_1 import Pycoproc

#For Pytrack board
from LIS2HH12 import LIS2HH12 #Accelerometer
from L76GNSS import L76GNSS #GPS
##------------

import config
#=======================
#===== CONSTANTS =======
#=======================
lora = config.lora
LOOP_INTERVAL = 10
WLAN = WLAN(mode=WLAN.STA)


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

#take values and log
def take_acc_values(acc):
    acceleration = acc.acceleration()
    pitch = acc.pitch()
    roll = acc.roll()

    print("Acceleration: " + str(acceleration))
    print("Roll: " + str(roll))
    print("Pitch: " + str(pitch))

    return acceleration, pitch, roll

def take_coordenates(l76):
    coord = l76.coordinates();
    print("Coordenates: {}".format(coord)) # (None, None) when using inside
    return coord

#Init board
def init_pytrack():
    py = Pycoproc(Pycoproc.PYTRACK)
    acc = LIS2HH12()
    l76 = L76GNSS(py, timeout=30)
    return acc, l76

#Network
def setup_rtc(rtc):
    print("RTC NTP sync")
    rtc.ntp_sync("pool.ntp.org")
    utime.sleep_ms(750)
    print("RTC set from NTP to UTC: {}".format(rtc.now()))
    utime.timezone(7200)
    print('Adjusted from UTC to Madrid timezone', utime.localtime(), '\n')

def init_wifi():
    WLAN.connect(ssid=config.WLAN_SSID, auth=(WLAN.WPA2, config.WLAN_PASS), timeout=config.WLAN_TIMEOUT_MS)
    print("-- Trying to connect to WLAN with SSID {} --".format(config.WLAN_SSID))

    while not WLAN.isconnected():
        pycom.rgbled(0xFF0000)
        time.sleep(1)
        pycom.rgbled(0x000000)
        time.sleep(1)
        print(".", end = '')
        machine.idle()

    print("\nWiFi connected succesfully")

def init_lora():
    lora.nvram_restore()

    if not lora.has_joined():
        print("-- Initializing LoRaWAN (Client) --")
        lora.join(activation=LoRa.OTAA, auth=(config.DEV_EUI,config.APP_EUI, config.APP_KEY), timeout=0)

        # wait until the module has joined the network
        while not lora.has_joined():
            pycom.rgbled(0xFF00FF)
            time.sleep(1.5)
            pycom.rgbled(0x000000)
            time.sleep(1)
            print(".", end = '')
            pycom.rgbled(0x000000)
            print("\nLoRa connected and authenticated succesfully")
    else:
        print("-- LoRaWAN (Client) already join !")

    print("Initializing socket")
    sckt = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
    #Config DataRate
    sckt.setsockopt(socket.SOL_LORA, socket.SO_DR, config.LORA_NODE_DR)
    #Config Confirmed or Non-Confirmed type of messages
    sckt.setsockopt(socket.SOL_LORA, socket.SO_CONFIRMED, config.CONFIRM_MESSAGES)

    return lora, sckt

#Data
def send_uplink(sckt, acceleration, pitch, roll, coord):
    #Acceleration wont send
    payload = "{}${}${}".format(roll, pitch, coord)
    print("Sending: {}".format(payload))
    sckt.setblocking(True)
    sckt.send(payload)
    sckt.setblocking(False)
    # store in flush to judge next time
    lora.nvram_save()
    # get any data received (if any...)
    if(config.CONFIRM_MESSAGES):
        time.sleep(1)
        receive_downlink(sckt)

def receive_downlink(sckt):
    print("->Reading downlink . . . ")
    downlink_message = sckt.recv(64)

    if downlink_message == b'':
        print("There is no downlink :(")
    else:
        downlink_payload = downlink_message.decode('ascii') # string form
        print("Downlink message: " + downlink_payload)
        pycom.rgbled(0x00FF00) # green
        time.sleep(1)
        pycom.rgbled(0x000000) # green

#Others
def wait_next_loop():
    pycom.rgbled(0xFFFFFF)
    time.sleep(1)
    pycom.rgbled(0x000000)
    machine.deepsleep(LOOP_INTERVAL)


#=======================
#======== MAIN =========
#=======================
def main():
    pycom.heartbeat(False)
    enable_gc();

    if(config.USE_WIFI):
        init_wifi()
        rtc = machine.RTC()
        setup_rtc(rtc=rtc)

    acc, l76=init_pytrack()

    config.get_dev_eui()
    lora, sckt = init_lora()

    while True:
        acceleration, pitch, roll = take_acc_values(acc=acc)
        coord = take_coordenates(l76)

        send_uplink(sckt, acceleration, pitch, roll, coord)

        print_separator()
        wait_next_loop()
    #End while
#End Main

#END MAIN & CALL
main();
