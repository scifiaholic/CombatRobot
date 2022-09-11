#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;
unsigned int localPort = 9999;

const char *ssid = "CounterStrike";  
const char *password = "legoman21";

IPAddress ipServer(192, 168, 4, 1);
IPAddress ipClient(192, 168, 4, 10);
IPAddress Subnet(255, 255, 255, 0);

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define YHPin 35
#define YVPin 34
#define YBPin 17

#define XHPin 36
#define XVPin 39
#define XBPin 5

#define UP1Pin 14
#define DN1Pin 27
#define LF1Pin 25
#define RT1Pin 26

#define UP2Pin 18
#define DN2Pin 16
#define LF2Pin 19
#define RT2Pin 4

void setup(){
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA); // ESP-32 as client
  WiFi.config(ipClient, ipServer, Subnet);
  Udp.begin(localPort);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  //display.setRotation(2);
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.print(F("Booting...."));
  display.display();

  pinMode(XHPin, INPUT);
  pinMode(XVPin, INPUT);
  pinMode(XBPin, INPUT);
  
  pinMode(YHPin, INPUT);
  pinMode(YVPin, INPUT);
  pinMode(YBPin, INPUT);
  
  pinMode(UP1Pin, INPUT);
  pinMode(DN1Pin, INPUT);
  pinMode(LF1Pin, INPUT);
  pinMode(RT1Pin, INPUT);
  
  pinMode(UP2Pin, INPUT);
  pinMode(DN2Pin, INPUT);
  pinMode(LF2Pin, INPUT);
  pinMode(RT2Pin, INPUT);
  
  Serial.begin(115200);
  Serial.println("booting...");
}

void loop(){  
  long UP1 = digitalRead(UP1Pin);
  long DN1 = digitalRead(DN1Pin);
  long LF1 = digitalRead(LF1Pin);
  long RT1 = digitalRead(RT1Pin);
  
  long UP2 = digitalRead(UP2Pin);
  long DN2 = digitalRead(DN2Pin);
  long LF2 = digitalRead(LF2Pin);
  long RT2 = digitalRead(RT2Pin);
  
  long XB = digitalRead(XBPin);
  long YB = digitalRead(YBPin);
  
  long YV = map(analogRead(YVPin), 0, 4095, -126, 127);
  long YH = map(analogRead(YHPin), 0, 4095, -126, 127);
  long XV = map(analogRead(XVPin), 0, 4095, -126, 127);
  long XH = map(analogRead(XHPin), 0, 4095, -126, 127);
  
  //SEND SERIAL DATA
  Serial.print(XV, DEC);
  Serial.print(", ");
  Serial.print(XH, DEC);
  Serial.print(", ");
  Serial.print(XB, DEC);
  Serial.print(", ");
  
  Serial.print(YV, DEC);
  Serial.print(", ");
  Serial.print(YH, DEC);
  Serial.print(", ");
  Serial.print(YB, DEC);
  
  Serial.print(", ");
  Serial.print(UP1, DEC);
  Serial.print(", ");
  Serial.print(DN1, DEC);
  Serial.print(", ");
  Serial.print(LF1, DEC);
  Serial.print(", ");
  Serial.print(RT1, DEC);
  
  Serial.print(", ");
  Serial.print(UP2, DEC);
  Serial.print(", ");
  Serial.print(DN2, DEC);
  Serial.print(", ");
  Serial.print(LF2, DEC);
  Serial.print(", ");
  Serial.print(RT2, DEC);
  
  //DISPLAY DATA
  display.clearDisplay();
  display.setCursor(0,0);

  display.print(F("X:"));
  display.print(XV, DEC);
  display.print(F(", "));
  display.print(XH, DEC);
  display.print(F(", "));
  display.println(XB, DEC);

  display.print(F("Y:"));
  display.print(YV, DEC);
  display.print(F(", "));
  display.print(YH, DEC);
  display.print(F(", "));
  display.println(YB, DEC);

  display.print(F("UP:"));
  display.print(UP1, DEC);
  display.print(F(" DN:"));
  display.print(DN1, DEC);
  display.print(F(" LF:"));
  display.print(LF1, DEC);
  display.print(F(" RT:"));
  display.println(RT1, DEC);

  display.print(F("UP:"));
  display.print(UP2, DEC);
  display.print(F(" DN:"));
  display.print(DN2, DEC);
  display.print(F(" LF:"));
  display.print(LF2, DEC);
  display.print(F(" RT:"));
  display.print(RT2, DEC);
  
  display.display();

  //UDP SEND DATA
  Udp.beginPacket(ipServer,localPort);   //Initiate transmission of data
  char Buf[100];   // buffer to hold the string to append
  //sprintf(Buf, "%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu\r\n", XH, XV, XB, YH, YV, YB, UP, DN, LF, RT);
  sprintf(Buf, "%i,%i,%i,%i,%i,%i\n", XV, XH, XB, YV, YH, YB);
  Udp.printf(Buf);
  Udp.endPacket();  // Close communication

  delay(20);
  
  Serial.println();
}
