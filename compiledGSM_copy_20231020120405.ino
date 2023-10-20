#include <SoftwareSerial.h>
#include <String.h>

// Define GSM module connections
#define GSM_RX_PIN 7  // RX pin for SIM800L
#define GSM_TX_PIN 8  // TX pin for SIM800L

// Define relay control pins
#define START_RELAY_PIN 2
#define STOP_RELAY_PIN 3

// Define water overflow sensor pin
#define WATER_SENSOR_PIN 4

// Store admin phone numbers
String adminNumbers[5];
int numAdmins = 0;

// Initialize GSM module
SoftwareSerial gsmSerial(GSM_TX_PIN, GSM_RX_PIN);
String message = "";

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  gsmSerial.begin(9600);

  // Initialize relay pins
  pinMode(START_RELAY_PIN, OUTPUT);
  pinMode(STOP_RELAY_PIN, OUTPUT);

  // Initialize water sensor pin
  pinMode(WATER_SENSOR_PIN, INPUT);

  // Register admin phone numbers
  // Implement a function to add and manage admin numbers through SMS commands

  // Configure SIM800L for voice calls and SMS
  gsmSerial.println("AT+CMGF=1");  // Set SMS mode to text
  delay(1000);
  gsmSerial.println("AT+CLIP=1");  // Enable caller ID for incoming calls
  delay(1000);

  // Answer incoming calls automatically and set up call functionality
  gsmSerial.println("ATA");  // Answer calls automatically
  delay(1000);

  // Listen for incoming SMS and calls
  gsmSerial.println("AT+CLCC=1");  // Report incoming calls
  delay(1000);
}

void loop() {
  // Check for incoming SMS or call
  checkSMS();
  checkCall();

  // Check for water overflow
  if (digitalRead(WATER_SENSOR_PIN) == HIGH) {
    stopMotor();  // Water overflow detected, stop the motor
  }
}

void checkSMS() {
  if (gsmSerial.available()) {
    char c = gsmSerial.read();
    message += c;
    delay(10);
  }

  if (message.length() > 0) {
    Serial.println("Received SMS: " + message);

    // Parse and process the SMS command
    if (message.startsWith("1STN") && message.length() >= 13) {
      addAdmin(message.substring(5));
    } else {
      // Implement a function to parse and execute other SMS commands
    }

    message = "";  // Clear the message buffer
  }
}

void checkCall() {
  if (gsmSerial.available()) {
    String incomingCall = gsmSerial.readStringUntil('\n');
    if (incomingCall.startsWith("RING")) {
      // Extract the caller's phone number from the incoming call string
      String callerNumber = parseCallerNumber(incomingCall);

      // Handle the call based on the caller's number
      handleCall(callerNumber);
    }
  }
}

String parseCallerNumber(String callInfo) {
  // Implement parsing logic to extract the caller's phone number from the callInfo string
  // The format of callInfo may vary, so you need to adjust the parsing accordingly
  // Extract and return the caller's phone number
}

void handleCall(String callerNumber) {
  // Compare the caller's number with the admin numbers
  for (int i = 0; i < numAdmins; i++) {
    if (callerNumber.equals(adminNumbers[i])) {
      // Caller is an admin, perform desired action
      startMotor();  // Example: Start the motor for admins
      return;
    }
  }

  // If the caller is not an admin, you can implement other actions or restrictions here
}

void startMotor() {
  digitalWrite(START_RELAY_PIN, HIGH);
  delay(5000);  // Run the motor for 5 seconds
  digitalWrite(START_RELAY_PIN, LOW);
}

void stopMotor() {
  digitalWrite(STOP_RELAY_PIN, HIGH);
  delay(5000);  // Run the stop relay for 5 seconds
  digitalWrite(STOP_RELAY_PIN, LOW);
}

void addAdmin(String number) {
  if (numAdmins < 5) {
    adminNumbers[numAdmins] = number;
    numAdmins++;
    Serial.println("Added admin: " + number);
    // Send a confirmation SMS to the added admin number if needed
  } else {
    Serial.println("Admin list is full. Cannot add more.");
    // Send a notification SMS indicating that the admin list is full
  }
}

