import socket
import time

global Motor1A, Motor1B, Motor2A, Motor2B, Motor3A, Motor3B, Motor4A, Motor4B, Motor5A, Motor5B, Motor6A, Motor6B
global Servo1A, Servo2A, Servo3A, Servo4A

XBPast = 0
YBPast = 0

# Create a datagram socket and Bind to address and ip
UDPServerSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
UDPServerSocket.bind(('', 20001))
UDPServerSocket.setblocking(False)
print("UDP server up and listening")
 
# Listen for incoming datagrams and Send a reply to the client
while(True):
    message = bytearray(200)
    UDPServerSocket.readinto(message)
    msg = message.decode('ascii')
    msg = msg.split("\n")
    msg = msg[len(msg)-2]
    if len(msg) > 50:
        continue
    print("Message: %s" % msg)
    msglist = str(msg).split(",")
    XV = float(msglist[0])
    XH = float(msglist[1])
    XB = float(msglist[2])
    YV = float(msglist[3])
    YH = float(msglist[4])
    YB = float(msglist[5])
    
    if YV > 25:
        Motor3B.duty(0)
        Motor3A.duty(mapDuty(YV))
    elif YV < -25:
        Motor3B.duty(mapDuty(YV))
        Motor3A.duty(0)
    else:
        Motor3B.duty(0)
        Motor3A.duty(0)
        
    if XV > 25:
        Motor4B.duty(0)
        Motor4A.duty(mapDuty(XV))
    elif XV < -25:
        Motor4B.duty(mapDuty(XV))
        Motor4A.duty(0)
    else:
        Motor4B.duty(0)
        Motor4A.duty(0)

    XBPast = XB
    YBPast = YB
    