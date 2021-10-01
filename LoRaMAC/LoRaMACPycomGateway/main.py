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


#=======================
#===== CONSTANTS =======
#=======================
ENABLE_WIFI = True
WLAN_SSID = "foo" #WiFi SSID
WLAN_PASS = "123abc123" #WiFi Password
WLAN_TIMEOUT_MS = 180000

INFLUX_TOKEN = ""
INFLUX_WRITE_URL = ""
INFLUX_WRITE_ARGS = ""

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

#Network
def setup_rtc(rtc):
    print("RTC NTP sync")
    rtc.ntp_sync("pool.ntp.org")
    utime.sleep_ms(750)
    print("RTC set from NTP to UTC: {}".format(rtc.now()))
    utime.timezone(7200)
    print('Adjusted from UTC to Madrid timezone', utime.localtime(), '\n')

def init_lora():
    print("Initializing LoRa (MAC)")
    lora = LoRa(mode=LoRa.LORA, region=LoRa.EU868)
    sckt = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
    sckt.setblocking(False)

    return lora, sckt

def connect_wlan(wlan):
    print("Connecting to SSID: {}".format(WLAN_SSID))

    wlan.connect(
        ssid=WLAN_SSID,
        auth=(WLAN.WPA2, WLAN_PASS),
        timeout=WLAN_TIMEOUT_MS)

    now = time.ticks_ms()

    while not wlan.isconnected():
        if (time.ticks_ms() - now) > WLAN_TIMEOUT_MS:
            raise TimeoutError("WiFi timeout")

        print("Still disconnected")
        pycom.rgbled(0xFF0000)
        time.sleep(0.5)
        pycom.rgbled(0x000000)
        time.sleep(0.5)

    pycom.rgbled(0x000000)
    print("WiFi connected succesfully")
    print("wlan.ifconfig:\n{}".format(wlan.ifconfig()))

#Data
def parse_payload(payload):
    try:
        payload = payload.decode()
        splitted = payload.split(",")
        temp = float(splitted[0])
        hum = float(splitted[1])
        return {"temp": temp, "hum": hum}
    except Exception as ex:
        return None

def read_data(data):
    print("Received: {}".format(data))

def post_data_to_db(temp, hum):
    try:
        url = "{}?{}".format(INFLUX_WRITE_URL, INFLUX_WRITE_ARGS)
        device = ubinascii.hexlify(machine.unique_id()).decode()
        data = "device={} temp={} hum={}".format(
            device, temp, hum)

        print("Uploading: {}".format(data))

        res = requests.post(
            url,
            headers={
                "Content-Type": "text/plain",
                "Authorization": "Token {}".format(INFLUX_TOKEN)
            },
            data=str(data))

        res.close()
    except Exception as ex:
        print("HTTP error: {}".format(ex))

#Others
def wait_next_loop(wasReceiveMsg):
    if wasReceiveMsg:
        pycom.rgbled(0x00FF00)
    else:
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

    if ENABLE_WIFI:
        wlan = WLAN(mode=WLAN.STA)
        connect_wlan(wlan=wlan)

    lora, sckt = init_lora()
    rtc = machine.RTC()
    setup_rtc(rtc=rtc)

    wasReceiveMsg=False

    while True:
        payload = sckt.recv(64)
        data = parse_payload(payload=payload)

        if data:
            read_data(data)
            wasReceiveMsg = True
        else:
            wasReceiveMsg = False;

        #if ENABLE_WIFI:
        #    post_data_to_db(acceleration, pitch, roll, coord)

        wait_next_loop(wasReceiveMsg)
        #print_separator()
        #End If data
    #End while
#End Main

#END MAIN & CALL
main();
