#include <IRremote.h>
#include <BlynkSimpleEsp8266.h>
#include <AceButton.h>
using namespace ace_button;

BlynkTimer timer1;
                              // Define the GPIO connected with Relays
#define RelayPin1 D0  //D0
#define RelayPin2 D1  //D1
#define RelayPin3 D2  //D2
#define RelayPin4 D3  //D3
                              //Define the GPIO connected with Switches
#define SwitchPin1 D5  //D5
#define SwitchPin2 D6  //D6
#define SwitchPin3 1  //TX
#define SwitchPin4 3   //RX
                               //Define the GPIO shows Wi-Fi led & IR reciver pin
#define wifiLed     D4   //D4
#define IR_RECV_PIN D5  //D5

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4
#define VPIN_BUTTON_5    V5
#define VPIN_BUTTON_6    V6

int toggleState_1 = 1; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 1; //Define integer to remember the toggle state for relay 2
int toggleState_3 = 1; //Define integer to remember the toggle state for relay 3
int toggleState_4 = 1; //Define integer to remember the toggle state for relay 4

int wifiFlag = 0;

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);
ButtonConfig config3;
AceButton button3(&config3);
ButtonConfig config4;
AceButton button4(&config4);


void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);
void handleEvent3(AceButton*, uint8_t, uint8_t);
void handleEvent4(AceButton*, uint8_t, uint8_t);


#define AUTH "--------------------------"                 // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "------------------"             //Enter Wifi Name
#define WIFI_PASS "-----------------"         //Enter wifi Password

void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 1){
              digitalWrite(RelayPin1, LOW); // turn on relay 1
              toggleState_1 = 0;
              Serial.println("Device1 ON");
              }
             else{
              digitalWrite(RelayPin1, HIGH); // turn off relay 1
              toggleState_1 = 1;
              Serial.println("Device1 OFF");
              }
             delay(100);
      break;
      case 2: 
             if(toggleState_2 == 1){
              digitalWrite(RelayPin2, LOW); // turn on relay 2
              toggleState_2 = 0;
              Serial.println("Device2 ON");
              }
             else{
              digitalWrite(RelayPin2, HIGH); // turn off relay 2
              toggleState_2 = 1;
              Serial.println("Device2 OFF");
              }
             delay(100);
      break;
      case 3: 
             if(toggleState_3 == 1){
              digitalWrite(RelayPin3, LOW); // turn on relay 3
              toggleState_3 = 0;
              Serial.println("Device3 ON");
              }
             else{
              digitalWrite(RelayPin3, HIGH); // turn off relay 3
              toggleState_3 = 1;
              Serial.println("Device3 OFF");
              }
             delay(100);
      break;
      case 4: 
             if(toggleState_4 == 1){
              digitalWrite(RelayPin4, LOW); // turn on relay 4
              toggleState_4 = 0;
              Serial.println("Device4 ON");
              }
             else{
              digitalWrite(RelayPin4, HIGH); // turn off relay 4
              toggleState_4 = 1;
              Serial.println("Device4 OFF");
              }
             delay(100);
      break;
      default : break;      
      }  
}

void all_SwitchOff(){
  toggleState_1 = 1; digitalWrite(RelayPin1, toggleState_1); Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); delay(100);
  toggleState_2 = 1; digitalWrite(RelayPin2, toggleState_2); Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2); delay(100);
  toggleState_3 = 1; digitalWrite(RelayPin3, toggleState_3); Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3); delay(100);
  toggleState_4 = 1; digitalWrite(RelayPin4, toggleState_4); Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4); delay(100);
}

void all_SwitchOn(){
  toggleState_1 = 0; digitalWrite(RelayPin1, toggleState_1); Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); delay(100);
  toggleState_2 = 0; digitalWrite(RelayPin2, toggleState_2); Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2); delay(100);
  toggleState_3 = 0; digitalWrite(RelayPin3, toggleState_3); Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3); delay(100);
  toggleState_4 = 0; digitalWrite(RelayPin4, toggleState_4); Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4); delay(100);
}

void ir_remote(){
  if (irrecv.decode(&results)) {
      switch(results.value){
          case 0x----------:  relayOnOff(1); Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); break;
          case 0x----------:  relayOnOff(2); Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2); break;
          case 0x----------:  relayOnOff(3); Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3); break;
          case 0x---------:  relayOnOff(4); Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4); break;
          case 0x---------:  all_SwitchOn(); break;
          case 0x---------:  all_SwitchOff(); break;
          default : break;         
        }   
        //Serial.println(results.value, HEX);    
        irrecv.resume();   
  } 
}

// When App button is pushed - switch the state
BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, toggleState_2);
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, toggleState_3);
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, toggleState_4);
}


BLYNK_WRITE(VPIN_BUTTON_5) {
  all_SwitchOff();
}

BLYNK_WRITE(VPIN_BUTTON_6) {
  all_SwitchOn();
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    digitalWrite(wifiLed, LOW); //Turn off WiFi LED
  }
  if (isconnected == true) {
    wifiFlag = 0;    
    digitalWrite(wifiLed, HIGH); //Turn on WiFi LED
  }
}
void setup()
{
  Serial.begin(9600);
  
  irrecv.enableIRIn(); // Start the receiver
  
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);
  
  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, toggleState_1);
  digitalWrite(RelayPin2, toggleState_2);
  digitalWrite(RelayPin3, toggleState_3);
  digitalWrite(RelayPin4, toggleState_4);
  
  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  config3.setEventHandler(button3Handler);
  config4.setEventHandler(button4Handler);

  
  button1.init(SwitchPin1);
  button2.init(SwitchPin2);
  button3.init(SwitchPin3);
  button4.init(SwitchPin4);


  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer1.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
  Blynk.config(AUTH);
  delay(1000);

  Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
  Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
  Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
  Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);

}

void loop()
{      
  ir_remote();
  
  button1.check();
  button2.check();
  button3.check();
  button4.check();

  if (WiFi.status() == WL_CONNECTED)
  {
    Blynk.run();
  }
  
  timer1.run(); // Initiates SimpleTimer
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      toggleState_1 = 0;
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget
      digitalWrite(RelayPin1, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      toggleState_1 = 1;
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget
      digitalWrite(RelayPin1, HIGH);
      break;
  }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      toggleState_2 = 0;
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      digitalWrite(RelayPin2, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      toggleState_2 = 1;
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      digitalWrite(RelayPin2, HIGH);
      break;
  }
}

void button3Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT3");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      toggleState_3 = 0;
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
      digitalWrite(RelayPin3, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      toggleState_3 = 1;
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
      digitalWrite(RelayPin3, HIGH);
      break;
  }
}

void button4Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT4");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      toggleState_4 = 0;
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
      digitalWrite(RelayPin4, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      toggleState_4 = 1;
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
      digitalWrite(RelayPin4, HIGH);
      break;
  }
}
