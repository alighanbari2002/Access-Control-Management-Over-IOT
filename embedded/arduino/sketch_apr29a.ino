#include <EtherCard.h>
#include <ArduinoJson.h>
#include <Servo.h>
#define Green_LED_Port 9
#define Red_LED_Port 8
#define Door_Motor_Port 2
#define Access_Denied "Access Denied!"

#define REQUEST_RATE 5000 // milliseconds

Servo doormotor;

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

  Serial.println(off);

  // response = response.substring(0,14);

  Serial.println(response.length());
  String temp = Access_Denied;
  Serial.println(temp.length());
  Serial.println(response);
  
  // Check if the response is equal to a specific string
  if (response.equals(Access_Denied)) {
    Serial.println("hello we need boobies");
    digitalWrite(Red_LED_Port, HIGH);
    doormotor.write(0);
  } else {
    digitalWrite(Green_LED_Port, HIGH);
    doormotor.write(90);

  }
}

void setup () {
  pinMode(Green_LED_Port, OUTPUT);
  pinMode(Red_LED_Port, OUTPUT);
  doormotor.attach(Door_Motor_Port);
  doormotor.write(0);

  Serial.begin(9600);
  Serial.println("\n[getStaticIP]");

  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println( "Failed to access Ethernet controller");

  ether.staticSetup(myip, gwip);

  ether.copyIp(ether.hisip, hisip);
  ether.printIp("Server: ", ether.hisip);

  while (ether.clientWaitingGw())
    ether.packetLoop(ether.packetReceive());
  Serial.println("Gateway found");

  timer = - REQUEST_RATE; // start timing out right away
}

void loop () {
  ether.packetLoop(ether.packetReceive());

  if (millis() > timer + REQUEST_RATE) {
    timer = millis();
    Serial.println("\n>>> REQ");
    
    const char* myUrl = "/service"; // Your desired URL
    const char* myHost = "192.168.2.1"; // Your server's hostname
    //const char* myAdditionalHeader = "Content-Type: application/json"; // Additional header if needed
    // const char* myPostData = "{\"username\":\"Ahmad\"}"; // Your JSON data

    digitalWrite(Green_LED_Port, LOW);
    digitalWrite(Red_LED_Port, LOW);
    doormotor.write(0);

    if (Serial.available() > 0) {
     String input = Serial.readStringUntil('\r');
     
     // Print the input received
     Serial.print("Input from Virtual Terminal: ");
     Serial.println(input);
     StaticJsonDocument<200> jsonDocument;
     jsonDocument["username"] = input;
     
     // Serialize JSON document to a char array
     char jsonBuffer[200];
     serializeJson(jsonDocument, jsonBuffer);
     
     // Use the input as POST data
     ether.httpPost(PSTR("/service"), website, PSTR("Content-Type: application/json"), jsonBuffer, my_result_cb);
    //  ether.tcpSend();
     
    }
    // char payload[] = "My TCP message";
    // ether.httpPost(const char *urlbuf, const char *hoststr, const char *additionalheaderline, const char *postval, void (*callback)(uint8_t, uint16_t, uint16_t))
    // ether.tcpSend(payload, sizeof(payload), 80, 80, );
    // ether.sendUdp(const char *data, uint8_t len, uint16_t sport, const uint8_t *dip, uint16_t dport);
    // ether.clientTcpReq(uint8_t (*result_cb)(uint8_t, uint8_t, uint16_t, uint16_t), uint16_t (*datafill_cb)(uint8_t), uint16_t port)
  }
}

// char payload[] = "My UDP message";
// uint8_t nSourcePort = 1234;
// uint8_t nDestinationPort = 5678;
// uint8_t ipDestinationAddress[IP_LEN];
// ether.parseIp(ipDestinationAddress, "192.168.0.200");

// ether.sendUdp(payload, sizeof(payload), nSourcePort, ipDestinationAddress, nDestinationPort);


// void setup() {  // initialize digital pin 13 as an output.
//    pinMode(13, OUTPUT);
// }

// // the loop function runs over and over again forever

// void loop() {
//    digitalWrite(13, HIGH); // turn the LED on (HIGH is the voltage level)
//    delay(1000); // wait for a second
//    digitalWrite(13, LOW); // turn the LED off by making the voltage LOW
//    delay(1000); // wait for a second
// }


// #include <EtherCard.h>

// // Static IP configuration
// static byte myip[] = { 169, 254, 199, 139 }; // Arduino's static IP address
// static byte gwip[] = { 169, 254, 199, 140 }; // Gateway IP address

// // MAC address of the Arduino
// static byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 };

// // Ethernet buffer size
// byte Ethernet::buffer[700];

// // Welcome page HTML content (stored in PROGMEM)
// const char welcomePage[] PROGMEM =
//   "<!DOCTYPE html>"
//   "<html lang='en'>"
//   "<head>"
//   "<meta charset='UTF-8'>"
//   "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
//   "<title>Electronics Tree</title>"
//   "<style>"
//   "body {"
//   "font-family: Arial, sans-serif;"
//   "background-color: #f0f0f0;"
//   "text-align: center;"
//   "padding: 20px;"
//   "}"
//   "h1 {"
//   "color: #333333;"
//   "}"
//   "p {"
//   "color: #666666;"
//   "}"
//   "</style>"
//   "</head>"
//   "<body>"
//   "<h1>Electronics Tree's Hands-On Journey</h1>"
//   "<p>Welcome to Electronics Tree, where our name isn’t just a title—it’s a "
//   "commitment to practical growth in your knowledge and skills."
//   "Think of it like planting a seed.</p>"
//   "</body>"
//   "</html>";

// void setup() {
//   // Begin Ethernet communication with buffer size and MAC address
//   ether.begin(sizeof Ethernet::buffer, mymac, SS);

//   // Configure static IP and gateway IP
//   ether.staticSetup(myip, gwip);
// }

// void loop() {
//   // Handle incoming Ethernet packets and get the position of the data
//   word pos = ether.packetLoop(ether.packetReceive());

//   // If data is received
//   if (pos) {
//     // Extract data from Ethernet buffer
//     char *data = (char *)Ethernet::buffer + pos;

//     // Copy the welcome page content to the Ethernet buffer
//     memcpy_P(ether.tcpOffset(), welcomePage, sizeof welcomePage);

//     // Send HTTP response with the welcome page to the client
//     ether.httpServerReply(sizeof welcomePage - 1);
//   }
// }