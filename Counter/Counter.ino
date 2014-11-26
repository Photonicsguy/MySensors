// Simple counter for testing communication and logging
// VAR_1 increments every cycle
// VAR_2 increments on every transmission failure

#include <MySensor.h>
#include <SPI.h>
#include <EEPROM.h>

#define VREG 4  // Enable line for 3.3VREG

unsigned long t0;
unsigned long Counter = 0;
unsigned long Fail = 0;
bool sendOk;
bool hasfailed = false;
unsigned long SLEEP_TIME = 1000; // Sleep time between reads (in milliseconds)

MySensor gw;

boolean receivedConfig = false;
boolean metric = true;
// Initialize temperature message
MyMessage msg_cnt(0, V_VAR1);
MyMessage msg_err(1, V_VAR2);

void setup()
{
  digitalWrite(VREG, LOW);  // Turn off Radio
  pinMode(VREG, OUTPUT);

  Serial.begin(115200);
#ifdef AAA
  Serial.print("Node ID: ");
  Serial.println(EEPROM.read(0));
  Serial.print("Parent node ID: ");
  Serial.println(EEPROM.read(1));
  Serial.print("Distance: ");
  Serial.println(EEPROM.read(2));
  Serial.print("Route : me");
  for (int i = 0; i < EEPROM.read(2); i++) {
    Serial.print(" => ");
    Serial.print(EEPROM.read(3 + i), DEC);
  }
  Serial.println();
  delay(500);
#endif
  digitalWrite(VREG, HIGH);  // Turn on Radio

  // Startup and initialize MySensors library. Set callback for incoming messages.
  //gw.begin();
  gw.begin(incomingMessage, AUTO, true);

  // Send the sketch version information to the gateway and Controller
  gw.sendSketchInfo("Counting Sensor", "1.0");

  gw.present(0, S_CUSTOM);
  gw.present(1, S_CUSTOM);
  gw.send(msg_err.set(Fail, 0));
}


void loop()
{
  // Process incoming messages (like config from server)
  gw.process();

  sendOk = gw.send(msg_cnt.set(Counter++, 0));
  if (!sendOk) {
    Fail++;
    hasfailed = true;
  }
  if (hasfailed) {
    if (gw.send(msg_err.set(Fail, 0))) {
      hasfailed = false;
    }
  }
  Serial.print("Count: \t");
  Serial.print(Counter);
  Serial.print (" ");
  Serial.print (sendOk ? "ok" : "fail");
  Serial.print ("\t\t\tFail: \t");
  Serial.println(Fail);

  //  Serial.println("Starting delay timer");
  t0 = millis();
  while (((t0 + SLEEP_TIME) > millis()) && millis() >= t0) {
    gw.process();
  }
  Serial.print((millis() - t0) / 1000.0);
  Serial.println(" second delay\n\n");

}

// Comment out gw.sleep to use the following!!
void incomingMessage(const MyMessage &message) {
  Serial.print("Incoming message\n");
  //  if (message.type == V_TEMP) {
  //    lastTemperature[message.sensor] = -127.00;
  //    Serial.print("Data request for sensor ");
  //    Serial.println(message.sensor);
  //  }
}
