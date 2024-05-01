#include <Ethernet.h>

#include <Servo.h>
#include <WebSocketsClient.h>

#define Green_LED_Port 9
#define Red_LED_Port 8
#define Door_Motor_Port 2
#define Access_Denied "Access Denied!"

#define REQUEST_RATE 5000 // milliseconds


// ethernet interface mac address
// static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
// // ethernet interface ip address
// static byte myip[] = { 192,168,2,2 };
// static byte gwip[] = { 192,168,2,1 };
// static byte hisip[] = { 192,168,2,1 };
// const char website[] PROGMEM = "192.168.2.1";

Servo doormotor;
WebSocketsClient webSocket;


void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println(F("Disconnected!"));
            break;
        case WStype_CONNECTED:
            {
                Serial.print(F("Connected"));
                // Serial.println((char *)payload);
                // send message to server when Connected
                webSocket.sendTXT(F("Connected"));
            }
            break;
        case WStype_TEXT:
        {
          // Serial.print("<<< reply ");
          // Serial.print(millis() - timer);
          // Serial.println(" ms");
          Serial.println((char *)payload);
          String response = (char *)payload;

          if (response.equals(Access_Denied)) {
            Serial.println(Access_Denied);
            digitalWrite(Red_LED_Port, HIGH);
            doormotor.write(0);
          } else {
            Serial.println(F("Access Granted!"));
            digitalWrite(Green_LED_Port, HIGH);
            doormotor.write(90);
          }
        }
    }

}

void setup()
{
  byte mac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

// Set the static IP address to use if the DHCP fails to assign
  IPAddress ip(192, 168, 2, 2);
  pinMode(Green_LED_Port, OUTPUT);
  pinMode(Red_LED_Port, OUTPUT);
  doormotor.attach(Door_Motor_Port);
  doormotor.write(0);

  Serial.begin(9600);
  Serial.println("\n[getStaticIP]");

  Ethernet.begin(mac, ip);

  webSocket.begin("192.168.2.1", 80);
  webSocket.onEvent(webSocketEvent);

}


void loop()
{
    static long timer;
    webSocket.loop();
    if (millis() > timer + REQUEST_RATE) {
    timer = millis();
    Serial.println("\n>>> REQ");

    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\r');
      
      // Print the input received
      Serial.print("Input from Virtual Terminal: ");
      Serial.println(input);
      String in = String (input);
      // StaticJsonDocument<200> jsonDocument;
      // jsonDocument["username"] = input;
      
      // Serialize JSON document to a char array
      // char jsonBuffer[200];
      // serializeJson(jsonDocument, jsonBuffer);

      // Send JSON data over WebSocket
      webSocket.sendTXT(in);
    }
    }

}