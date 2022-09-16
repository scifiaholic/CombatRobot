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
  
def mapDuty(val):
    val = int(map(abs(val), 0, 127, 0, 1023))
    if val > 1023:
        val = 1023
    elif val < 0:
        val = 0
    return val

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
    print("waiting for connection")

import webrepl
webrepl.start()

Motor1A = PWM(Pin(23), freq=50, duty=0)
Motor1B = PWM(Pin(22), freq=50, duty=0)
Motor2A = PWM(Pin(21), freq=50, duty=0)
Motor2B = PWM(Pin(19), freq=50, duty=0)
Motor3A = PWM(Pin(18), freq=50, duty=0)
Motor3B = PWM(Pin(5 ), freq=50, duty=0)
Motor4A = PWM(Pin(17), freq=50, duty=0)
Motor4B = PWM(Pin(16), freq=50, duty=0)
Motor5A = PWM(Pin(4 ), freq=50, duty=0)
Motor5B = PWM(Pin(2 ), freq=50, duty=0)
Motor6A = PWM(Pin(27), freq=50, duty=0)
Motor6B = PWM(Pin(14), freq=50, duty=0)

Servo1A = PWM(Pin(26), freq=50, duty=0)
Servo2A = PWM(Pin(25), freq=50, duty=0)
Servo3A = PWM(Pin(33), freq=50, duty=0)
#Servo4A = PWM(Pin(32), freq=50, duty=0)