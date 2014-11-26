
//#define  WDT

#include <MySensor.h>
#include <SPI.h>
#include <Wire.h>
#include <AM2321.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <EEPROM.h>
#ifdef WDT
#include <avr/wdt.h>
#endif
#include <Weather.h>

#define VREG 4  // Enable line for 3.3VREG

//#define DEBUG


#define OFFSET 0.97941    // Offset to match with Enviroment Canada

unsigned long t0;
unsigned long Fail = 0;
bool sendOk;
bool hasfailed = false;
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)

unsigned int last_fail = -1;
int last_temp = -100;
float temp;
unsigned int last_hum = 101;
unsigned long t0_temp;
unsigned long t0_hum;
unsigned long t0_time;

MySensor gw;
AM2321 ac;
//Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

boolean receivedConfig = false;
boolean metric = true;


// Initialize messages
MyMessage msg_fail(0, V_VAR2);  // Errors
MyMessage msg_temp(1, V_TEMP); // Temperature
MyMessage msg_hum(2, V_HUM);   // Humidity


void setup() {
#ifdef WDT
  wdt_enable(WDTO_2S);
  wdt_reset();
#endif
  digitalWrite(VREG, LOW);  // Turn off Radio
  pinMode(VREG, OUTPUT);

  Serial.begin(115200);
#ifdef DEBUG
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

#ifdef WDT
  wdt_reset();
#endif
  digitalWrite(VREG, HIGH);  // Turn on Radio

  // Startup and initialize MySensors library. Set callback for incoming messages.
  //gw.begin();
  gw.begin(incomingMessage, AUTO, true);

  // Send the sketch version information to the gateway and Controller
  gw.sendSketchInfo("AM2320B Node", "1.0");
#ifdef WDT
  wdt_reset();
#endif
  gw.present(0, S_CUSTOM);
  gw.present(1, S_TEMP);
  gw.present(1, S_HUM);
#ifdef WDT
  wdt_reset();
#endif
}


void loop()
{
#ifdef WDT
  wdt_reset();
#endif
  // Process incoming messages (like config from server)
  gw.process();

  if (((t0_time + 3600000) < millis()) || millis() <= t0_time) { // Also send data once an hour (or if millis() overflows (every 50 days)
    Serial.print("Sending hourly data, t=");
    Serial.print(millis() / 1000);
    Serial.println("s");
    last_hum++;  // Force transmission
    last_temp++;  // Force transmission
    last_fail++;  // Force transmission
    t0_time = millis();
  }

  if (ac.read()) {
    temp=ac.temperature/10.0;
    temp=10*((int)((temp*2.0)+0.5)/2.0);
    if (last_temp != temp) {
      if (gw.send(msg_temp.set(temp / 10.0, 1))) {
        last_temp = temp;
        t0_temp = millis();
      } else {
        Fail++;
      }
    }
    
    
    if (last_hum != ac.humidity/10) {
      if (gw.send(msg_hum.set(ac.humidity / 10,0))) {
        last_hum = ac.humidity/10;
        t0_hum = millis();
      } else {
        Fail++;
      }
    }
    if (last_fail != Fail) {
      if (gw.send(msg_fail.set(Fail, 0))) {
        last_fail = Fail;
      } else {
        Fail++;
      }
    }
  }

  Serial.print("Temperature: ");
  Serial.print(temp/10.0);
  Serial.print (" C\tHumidity: ");
  Serial.print(ac.humidity / 10);
  Serial.print (" %\t\t\tFail: ");
  Serial.println (Fail);
#ifdef WDT
  wdt_reset();
#endif
  //  Serial.println("Starting delay timer");
  t0 = millis();
  while (((t0 + SLEEP_TIME) > millis()) && millis() >= t0) {
#ifdef WDT
    wdt_reset();
#endif
    gw.process();
  }
  Serial.print((millis() - t0) / 1000.0);
  Serial.println(" second delay\n\n");

}

// Comment out gw.sleep to use the following!!
void incomingMessage(const MyMessage &message) {
  Serial.print("\n===================================\n");
  Serial.print("Incoming message\n");
  Serial.print("Message type: ");
  Serial.println(message.type);
  Serial.print("Sensor: ");
  Serial.println(message.sensor);
  Serial.print("Payload: ");
  Serial.println(message.getString());
  Serial.print("===================================\n\n");
}
