# This file is executed on every boot (including wake-boot from deep sleep)
#import esp
#esp.osdebug(None)
from os import listdir
from machine import Pin, PWM, reset
import machine
import micropython
import esp
import network
import time

def map(x, in_min, in_max, out_min, out_max):
  return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min
  
#default make sure an AP is available
ap = network.WLAN(network.AP_IF)
ap.config(essid='ESPController')
ap.active(True)
#connect to home
sta = network.WLAN(network.STA_IF)
sta.active(True)
sta.connect('GHome', '#1Marriage08')

while sta.isconnected() == False:
    time.sleep(1)
    print("Waiting for WiFi")

import webrepl
webrepl.start()