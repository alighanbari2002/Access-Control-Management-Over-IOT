#include <EtherCard.h>
#include <ArduinoJson.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#define Green_LED_Port 9
#define Door_Motor_Port 2
#define Access_Denied "Access Denied!"
#define Access_Granted "Access Granted"


#define REQUEST_RATE 5000 // milliseconds

Servo doormotor;
String rfid;
LiquidCrystal lcd(7, 8, 6, 5, 4, 3);

// ethernet interface mac address
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
// ethernet interface ip address
static byte myip[] = { 192,168,2,2 };
// gateway ip address
static byte gwip[] = { 192,168,2,1 };
// remote website ip address and port
static byte hisip[] = { 192,168,2,1 };
// remote website name
const char website[] PROGMEM = "192.168.2.1";

byte Ethernet::buffer[300];   // a very small tcp/ip buffer is enough here
static long timer;

// called when the client request is complete
static void my_result_cb (byte status, word off, word len) {
  Serial.print("<<< reply ");
  Serial.print(millis() - timer);
  Serial.println(" ms");
  String response = String((const char*)Ethernet::buffer + off);

  int size = response.length();
  response = response.substring(size - 14,size);

  if (response.equals(Access_Denied)) {
    Serial.println(Access_Denied);
    lcd.clear();
    lcd.print(Access_Denied);
    
    doormotor.write(0);
  } else if(response.equals(Access_Granted)){
    digitalWrite(Green_LED_Port, HIGH);
    doormotor.write(90);
    Serial.println(rfid);
    lcd.clear();
    lcd.print(rfid);
  }
}

void setup () {
  pinMode(Green_LED_Port, OUTPUT);

  doormotor.attach(Door_Motor_Port);
  doormotor.write(0);

  lcd.begin(16, 2);

  Serial.begin(9600);
  Serial.println("\n[getStaticIP]");

  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println( "Failed to access Ethernet controller");

  ether.staticSetup(myip, gwip);

  ether.copyIp(ether.hisip, hisip);
  ether.printIp("Server: ", ether.hisip);

  while (ether.clientWaitingGw())
    ether.packetLoop(ether.packetReceive());
  Serial.println("Gateway found");

  timer = - REQUEST_RATE; 
}

void loop () {
  ether.packetLoop(ether.packetReceive());

  if (millis() > timer + REQUEST_RATE) {
    timer = millis();
    Serial.println("\n>>> REQ");

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Waiting for");
    lcd.setCursor(0, 1);
    lcd.print("request");

    const char* myUrl = "/service"; // Your desired URL
    const char* myHost = "192.168.2.1"; // Your server's hostname

    digitalWrite(Green_LED_Port, LOW);
    // digitalWrite(Red_LED_Port, LOW);
    doormotor.write(0);

    if (Serial.available() > 0) {
     String input = Serial.readStringUntil('\r');
     
     Serial.print("Input from Virtual Terminal: ");
     Serial.println(input);
     rfid = input;

     StaticJsonDocument<200> jsonDocument;
     jsonDocument["username"] = input;
     
     char jsonBuffer[200];
     serializeJson(jsonDocument, jsonBuffer);
     
     ether.httpPost(PSTR("/service"), website, PSTR("Content-Type: application/json"), jsonBuffer, my_result_cb);
     
    }
  }
}