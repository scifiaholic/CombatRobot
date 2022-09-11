#include <WiFi.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

#define LO 1638
#define MD 3272
#define HI 3700

//GPIO pins for the motors
#define Motor1pinA 16
#define Motor1pinB 5
#define Motor2pinA 4
#define Motor2pinB 17
#define Motor3pinA 25
#define Motor3pinB 26
#define Motor4pinA 13
#define Motor4pinB 12
#define Servo1pinA 19
#define Servo2pinA 21
#define Servo3pinA 22
#define Servo4pinA 23

//Asigned channels of sigma Delta function for motors
int Motor1A = 0;
int Motor1B = 1;
int Motor2A = 2;
int Motor2B = 3;
int Motor3A = 4;
int Motor3B = 5;
int Motor4A = 6;
int Motor4B = 7;

int Servo1A = 8;
int Servo2A = 9;
int Servo3A = 10;
int Servo4A = 11;

uint32_t Servo1State = MD;
uint32_t Servo2State = MD;
uint32_t Servo3State = MD;
uint32_t Servo4State = MD;
uint32_t ReadState = 0;

//GPIO pins for the motor encoders
#define Encoder1pin A0
#define Direction1pin A3
#define Encoder2pin A6
#define Direction2pin A7
#define Encoder3pin 32
#define Direction3pin 33
#define Encoder4pin 27
#define Direction4pin 14

WiFiUDP Udp; // Creation of wifi Udp instance

char packetBuffer[512];
#define CTRLCNT 6
int XV = 0;
int XH = 0;
int XB = 0;
int XBpast = 0;
int YV = 0;
int YH = 0;
int YB = 0;
int YBpast = 0;
int CTRL[CTRLCNT] = {0, 0, 0, 0, 0, 0};

unsigned int localPort = 9999;

const char *ssid = "CounterStrike";
const char *password = "legoman21";

unsigned long previousTime = 0;
unsigned long currentTime = 0;
#define WAIT 500


void setup() {
  //intialize GPIO pins for motors
  pinMode(Motor1pinA, OUTPUT);
  pinMode(Motor1pinB, OUTPUT);
  pinMode(Motor2pinA, OUTPUT);
  pinMode(Motor2pinB, OUTPUT);
  pinMode(Motor3pinA, OUTPUT);
  pinMode(Motor3pinB, OUTPUT);
  pinMode(Motor4pinA, OUTPUT);
  pinMode(Motor4pinB, OUTPUT);

  pinMode(Servo1pinA, OUTPUT);
  pinMode(Servo2pinA, OUTPUT);
  pinMode(Servo3pinA, OUTPUT);
  pinMode(Servo4pinA, OUTPUT);

  //Servo PWM with assotiated motor GPIO
  ledcAttachPin(Motor1pinA, Motor1A);
  ledcAttachPin(Motor1pinB, Motor1B);
  ledcAttachPin(Motor2pinA, Motor2A);
  ledcAttachPin(Motor2pinB, Motor2B);
  ledcAttachPin(Motor3pinA, Motor3A);
  ledcAttachPin(Motor3pinB, Motor3B);
  ledcAttachPin(Motor4pinA, Motor4A);
  ledcAttachPin(Motor4pinB, Motor4B);

  ledcAttachPin(Servo1pinA, Servo1A);
  ledcAttachPin(Servo2pinA, Servo2A);
  ledcAttachPin(Servo3pinA, Servo3A);
  ledcAttachPin(Servo4pinA, Servo4A);

  ledcSetup(Motor1A, 50, 8);
  ledcSetup(Motor1B, 50, 8);
  ledcSetup(Motor2A, 50, 8);
  ledcSetup(Motor2B, 50, 8);
  ledcSetup(Motor3A, 50, 8);
  ledcSetup(Motor3B, 50, 8);
  ledcSetup(Motor4A, 50, 8);
  ledcSetup(Motor4B, 50, 8);

  ledcSetup(Servo1A, 50, 16);
  ledcSetup(Servo2A, 50, 16);
  ledcSetup(Servo3A, 50, 16);
  ledcSetup(Servo4A, 50, 16);
  
  ledcWrite(Motor1A, 0);
  ledcWrite(Motor1B, 0);
  ledcWrite(Motor2A, 0);
  ledcWrite(Motor2B, 0);
  ledcWrite(Motor3A, 0);
  ledcWrite(Motor3B, 0);
  ledcWrite(Motor4A, 0);
  ledcWrite(Motor4B, 0);

  ledcWrite(Servo1A, MD);
  ledcWrite(Servo2A, MD);
  ledcWrite(Servo3A, MD);
  ledcWrite(Servo4A, MD);

  //initialize GPIO pins for motor encoders
  pinMode(Encoder1pin, INPUT);
  pinMode(Direction1pin, INPUT);
  pinMode(Encoder2pin, INPUT);
  pinMode(Direction2pin, INPUT);
  pinMode(Encoder3pin, INPUT);
  pinMode(Direction3pin, INPUT);
  pinMode(Encoder4pin, INPUT);
  pinMode(Direction4pin, INPUT);

  //ReadState to EEPROM
  ReadState = EEPROM.read(0);
  if(ReadState == 1){
    Servo4State = HI;//constrain(map(YH, -126, 127, LO, HI), LO, HI);
    Servo3State = HI;//constrain(map(YH, -126, 127, LO, HI), LO, HI);
  }else{
    Servo4State = MD;
    Servo3State = MD;
  }

  //initialize communcations
  Serial.begin(115200);
  Serial.println("booting...");
  WiFi.softAP(ssid, password);  // ESP-32 as access point
  Udp.begin(localPort);
  Serial.print("UDP initialized at ");
  Serial.println(localPort);
  previousTime = millis();
}

void loop() {
  //Listen for messages from the controller.
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    previousTime = millis();

    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;

    if (packetBuffer[0] != 'C') {
      char * pch;
      pch = strtok(packetBuffer, ",");
      for (int i = 0; i < CTRLCNT; i++) {
        CTRL[i] = atoi(pch);
        pch = strtok(NULL, ",");
      }
      XV = CTRL[0];
      XH = CTRL[1];
      XB = CTRL[2];
      YV = CTRL[3];
      YH = CTRL[4];
      YB = CTRL[5];

      Serial.printf(" X- V:%i, H:%i, B:%i, Y- V:%i, H:%i, B:%i, S3:%i, S4:%i \n", XV, XH, XB, YV, YH, YB, Servo3State, Servo4State);
    } else Serial.println("Connected...");

    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.printf(" received\n");
    Udp.endPacket();
  }

  currentTime = millis();
  //Serial.printf("Time:%i\n", currentTime - previousTime);
  if (currentTime - previousTime > WAIT) {
    //No messages means kill all functions
    Serial.printf(" No Message! \n");
    ledcWrite(Motor1A, 0);
    ledcWrite(Motor1B, 0);
    ledcWrite(Motor2A, 0);
    ledcWrite(Motor2B, 0);
    ledcWrite(Motor3A, 0);
    ledcWrite(Motor3B, 0);
    ledcWrite(Motor4A, 0);
    ledcWrite(Motor4B, 0);

    Servo4State = MD;
    Servo3State = MD;
    ReadState = 0;
      
    ledcWrite(Servo1A, MD);
    ledcWrite(Servo2A, MD);
    ledcWrite(Servo3A, MD);
    ledcWrite(Servo4A, MD);
  } else {
    
    if(YV > 0){
      ledcWrite(Motor3B, 0);
      ledcWrite(Motor3A, 2*YV);
    }else{
      ledcWrite(Motor3B, 2*abs(YV));
      ledcWrite(Motor3A, 0);
    }
    
    if(XV > 0){
      ledcWrite(Motor4A, 0);
      ledcWrite(Motor4B, 2*XV);
    }else{
      ledcWrite(Motor4A, 2*abs(XV));
      ledcWrite(Motor4B, 0);
    }
    
    if (XB == 1 && XBpast == 0 && YB == 1) {
      if(ReadState == 0) {ReadState = 1; EEPROM.write(0,ReadState);}
      else {ReadState = 0; EEPROM.write(0,ReadState);}
    }
    if(ReadState == 1){
      Servo4State = HI;//constrain(map(YH, -126, 127, LO, HI), LO, HI);
      Servo3State = HI;//constrain(map(YH, -126, 127, LO, HI), LO, HI);
    }else{
      Servo4State = MD;
      Servo3State = MD;
    }
    ledcWrite(Servo4A, Servo4State);
    ledcWrite(Servo3A, Servo3State);
    XBpast = XB;
    
  }
}
