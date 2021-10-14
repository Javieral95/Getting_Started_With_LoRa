import sys
import gc
import socket
import time

import machine
import pycom
import utime

import urequests
import ubinascii

from network import LoRa

from pycoproc_1 import Pycoproc
from LIS2HH12 import LIS2HH12 #Acelerometro
from L76GNSS import L76GNSS #GPS


#=======================
#===== CONSTANTS =======
#=======================
LOOP_INTERVAL = 1


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

#Network
def setup_rtc(rtc):
    print("RTC NTP sync")
    rtc.ntp_sync("pool.ntp.org")
    utime.sleep_ms(750)
    print("RTC set from NTP to UTC: {}".format(rtc.now()))
    utime.timezone(7200)
    print('Adjusted from UTC to Madrid timezone', utime.localtime(), '\n')

def init_lora():
    print("Initializing LoRaWAN (Client)")
    lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)
    sckt = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
    sckt.setblocking(False)

    return lora, sckt

#Data
def post_lora_data(acceleration, pitch, roll, coord):
    #Acceleration wont send
    payload = "{},{},{}".format(roll, pitch, coord)
    print("Sending: {}".format(payload))
    sckt.send(payload)

#Others
def wait_next_loop():
    pycom.rgbled(0xFFFFFF)
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

    rtc = machine.RTC()
    setup_rtc(rtc=rtc)

    acc = LIS2HH12()
    l76 = L76GNSS(pycoproc, timeout=30)

    while True:
        acceleration, pitch, roll = take_acc_values(acc=acc)
        coord = take_coordenates(l76)

        post_lora_data(acceleration, pitch, roll, coord)

        wait_next_loop()
        print_separator()
    #End while
#End Main

#END MAIN & CALL
main();
