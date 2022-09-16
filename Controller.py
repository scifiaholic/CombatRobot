from machine import Pin, SoftI2C, ADC
import ssd1306
from time import sleep
import socket

address = "192.168.1.218"
localPort = 9999
MaxValue = 65535

SCREEN_WIDTH = 128
SCREEN_HEIGHT = 64

YHPin = 35
YVPin = 34
YBPin = 17

XHPin = 36
XVPin = 39
XBPin = 5

UP1Pin = 14
DN1Pin = 27
LF1Pin = 25
RT1Pin = 26

UP2Pin = 18
DN2Pin = 16
LF2Pin = 19
RT2Pin = 4

UDPClientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
UDPClientSocket.setblocking(False)

i2c = SoftI2C(scl=Pin(22), sda=Pin(21))
display = ssd1306.SSD1306_I2C(SCREEN_WIDTH, SCREEN_HEIGHT, i2c)

display.text('booting...', 0, 0)
display.show()


XH = Pin(XHPin, Pin.IN)
XV = Pin(XVPin, Pin.IN)
XHadc = ADC(XH, atten=ADC.ATTN_11DB)
XVadc = ADC(XV, atten=ADC.ATTN_11DB)

YH = Pin(YHPin, Pin.IN)
YV = Pin(YVPin, Pin.IN)
YHadc = ADC(YH, atten=ADC.ATTN_11DB)
YVadc = ADC(YV, atten=ADC.ATTN_11DB)

XB = Pin(XBPin, Pin.IN)
YB = Pin(YBPin, Pin.IN)

UP1 = Pin(UP1Pin, Pin.IN)
DN1 = Pin(DN1Pin, Pin.IN)
LF1 = Pin(LF1Pin, Pin.IN)
RT1 = Pin(RT1Pin, Pin.IN)

UP2 = Pin(UP2Pin, Pin.IN)
DN2 = Pin(DN2Pin, Pin.IN)
LF2 = Pin(LF2Pin, Pin.IN)
RT2 = Pin(RT2Pin, Pin.IN)

while True:
  XHval = XHadc.read_u16() * 255 // MaxValue - 128
  XVval = XVadc.read_u16() * 255 // MaxValue - 128
  
  YHval = YHadc.read_u16() * 255 // MaxValue - 128
  YVval = YVadc.read_u16() * 255 // MaxValue - 128
  
  XBval = XB.value()
  YBval = YB.value()
  
  UP1val = UP1.value()
  DN1val = DN1.value()
  LF1val = LF1.value()
  RT1val = RT1.value()

  UP2val = UP2.value()
  DN2val = DN2.value()
  LF2val = LF2.value()
  RT2val = RT2.value()
  
  display.fill(0)
  display.text( "X:" + str(XVval) + ", " + str(XHval) + ", " + str(XBval), 0, 0)
  display.text("U:" + str(UP2val) + " D:" + str(DN2val) + " L:" + str(LF2val) + " R:" + str(RT2val), 0, 9)
  display.text( "Y:" + str(YVval) + ", " + str(YHval) + ", " + str(YBval), 0, 20)
  display.text("U:" + str(UP1val) + " D:" + str(DN1val) + " L:" + str(LF1val) + " R:" + str(RT1val), 0, 29)
  display.show()
  
  msg = '%i,%i,%i,%i,%i,%i\n' % (XVval, XHval, XBval, YVval, YHval, YBval)
  print("Sending: %s To: %s" %(msg,address))
  print
  UDPClientSocket.sendto(str.encode(msg), (address,20001))
    
