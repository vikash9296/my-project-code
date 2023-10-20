
#include <IRremote.h>
 
int IR_RECV_PIN = D5;
 
IRrecv irrecv(IR_RECV_PIN);
decode_results results;
 
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}
 
void loop()
{
  if (irrecv.decode(&results))
  {
  Serial.println(results.value, HEX); //print the HEX code
  irrecv.resume();
  }
}
