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

#For PySense board
from LIS2HH12 import LIS2HH12 #Accelerometer
from LTR329ALS01 import LTR329ALS01 #Light Sensor
from MPL3115A2 import MPL3115A2, ALTITUDE #Pressure Sensor, Altitude to use meters instead of pascals
from SI7006A20 import SI7006A20 #Temperature Humidity Sensor
##------------

import config
#=======================
#===== CONSTANTS =======
#=======================
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

def take_alt_values(alt):
    temperature = alt.temperature()
    altitude = alt.altitude()
    print("Altitude: " + str(altitude))
    return altitude

def take_dht_values(dht):
    temperature = dht.temperature()
    dew_point = dht.dew_point()
    print("Temperature: " + str(temperature))
    print("Dew_point: " + str(dew_point))
    return temperature, dew_point

def take_li_values(li):
    light = li.light()
    print("Light: " + str(light))
    return light

#Init board
def init_pysense():
    py = Pycoproc(Pycoproc.PYSENSE)
    acc = LIS2HH12(py)
    alt = MPL3115A2(py,mode=ALTITUDE) #ALTITUDE=meters, PRESSURE=pascals
    dht = SI7006A20(py)
    li = LTR329ALS01(py)
    return acc, alt, dht, li

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
    print("-- Initializing LoRaWAN (Client) --")
    lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)
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

    sckt = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
    #Config DataRate
    sckt.setsockopt(socket.SOL_LORA, socket.SO_DR, config.LORA_NODE_DR)
    #Config Confirmed or Non-Confirmed type of messages
    sckt.setsockopt(socket.SOL_LORA, socket.SO_CONFIRMED, config.CONFIRME_MESSAGES)

    return lora, sckt

#Data
def post_lora_data(sckt, acceleration, pitch, roll, altitude, temperature,dew_point, light):
    #Acceleration wont send and dew_point
    payload = "{}${}${}${}${}".format(roll, pitch, altitude, temperature, light)
    print("Sending: {}".format(payload))
    sckt.setblocking(True)
    sckt.send(payload)
    sckt.setblocking(False)
    # get any data received (if any...)
    if(config.CONFIRM_MESSAGES):
        print("->Response: ")
        data = sckt.recv(64)
        print(data)

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

    if(config.USE_WIFI):
        init_wifi()
        rtc = machine.RTC()
        setup_rtc(rtc=rtc)

    acc, alt, dht, li=init_pysense()

    config.get_dev_eui()
    lora, sckt = init_lora()

    while True:
        acceleration, pitch, roll = take_acc_values(acc)
        altitude = take_alt_values(alt)
        temperature, dew_point = take_dht_values(dht)
        light = take_li_values(li)

        post_lora_data(sckt, acceleration, pitch, roll, altitude, temperature, dew_point, light)

        wait_next_loop()
        print_separator()
    #End while
#End Main

#END MAIN & CALL
main();
