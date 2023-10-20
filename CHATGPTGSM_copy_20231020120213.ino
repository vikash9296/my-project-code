#include <SoftwareSerial.h>

#define SIM800_RX_PIN 10
#define SIM800_TX_PIN 11

SoftwareSerial gsmSerial(SIM800_RX_PIN, SIM800_TX_PIN);

#define PUMP_ON_RELAY_PIN 2
#define PUMP_OFF_RELAY_PIN 3
#define WATER_FLOW_SENSOR_PIN 4
#define ADMIN_MOBILE_NUMBER "admin_number_here"

const int MAX_MOBILE_NUMBERS = 5;
String mobileNumbers[MAX_MOBILE_NUMBERS];
int mobileNumberCount = 0;

boolean tankFull = false; // Flag to indicate tank full condition

void setup() {
  pinMode(PUMP_ON_RELAY_PIN, OUTPUT);
  pinMode(PUMP_OFF_RELAY_PIN, OUTPUT);
  pinMode(WATER_FLOW_SENSOR_PIN, INPUT_PULLUP);
  
  gsmSerial.begin(9600);
  Serial.begin(9600);

  // Add admin mobile number
  mobileNumbers[mobileNumberCount++] = ADMIN_MOBILE_NUMBER;

  sendInitialStatusSMS("POWER ON");
}

void loop() {
  if (gsmSerial.available()) {
    String command = gsmSerial.readStringUntil('\n');
    command.trim();
    
    if (command == "PUMP ON") {
      startPump();
      delay(5000);
      stopPump();
      sendStatusSMS("PUMP ON");
    } else if (command == "PUMP OFF") {
      stopPump();
      delay(5000);
      sendStatusSMS("PUMP OFF");
    } else if (command.startsWith("ADD ")) {
      if (mobileNumberCount < MAX_MOBILE_NUMBERS) {
        String newNumber = command.substring(4);
        mobileNumbers[mobileNumberCount++] = newNumber;
        sendSMS("Added control for " + newNumber);
      } else {
        sendSMS("Maximum number of control numbers reached.");
      }
    }
  }
  
  // Check for manual switch activation and water flow sensor
  if (digitalRead(WATER_FLOW_SENSOR_PIN) == LOW) {
    stopPump(); // Stop the pump
    delay(5000);
    sendStatusSMS("TANK FULL - PUMP OFF"); // Send status SMS
    while (digitalRead(WATER_FLOW_SENSOR_PIN) == LOW) {
      // Wait until the water flow sensor is released
    }
  } else {
    // Check for water flow sensor activation
    if (waterFlowDetected()) {
      stopPump(); // Stop the pump
      delay(5000);
      sendStatusSMS("TANK FULL - PUMP OFF"); // Send status SMS
      resetWaterFlowSensor(); // Reset the water flow sensor state
    }
  }
}

void startPump() {
  digitalWrite(PUMP_ON_RELAY_PIN, HIGH);
  digitalWrite(PUMP_OFF_RELAY_PIN, LOW);
}

void stopPump() {
  digitalWrite(PUMP_ON_RELAY_PIN, LOW);
  digitalWrite(PUMP_OFF_RELAY_PIN, HIGH);
}

void sendStatusSMS(const char* status) {
  for (int i = 0; i < mobileNumberCount; i++) {
    sendSMS("Status for " + mobileNumbers[i] + ": " + status);
  }
}

void sendSMS(String message) {
  for (int i = 0; i < mobileNumberCount; i++) {
    gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text
    delay(100);
    gsmSerial.print("AT+CMGS=\"");
    gsmSerial.print(mobileNumbers[i]);
    gsmSerial.println("\"");
    delay(100);
    gsmSerial.print(message);
    delay(100);
    gsmSerial.write(26); // Send the Ctrl+Z character to indicate end of SMS
    delay(1000);
  }
}

// Simulate water flow detection
boolean waterFlowDetected() {
  // Replace this with your actual water flow detection logic
  // Return true when water flow is detected, false otherwise
  return false;
}

// Reset water flow sensor state
void resetWaterFlowSensor() {
  // Reset the water flow sensor state here
}

void sendInitialStatusSMS(const char* status) {
  for (int i = 0; i < mobileNumberCount; i++) {
    sendSMS("Status for " + mobileNumbers[i] + ": " + status);
  }
}
