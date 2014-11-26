// Read EEPROM configuration for MySensor nodes
// http://mysensors.org/

//

#include <EEPROM.h>
// EEPROM start address for mysensors library data
#define EEPROM_START 0
// EEPROM location of node id
#define EEPROM_NODE_ID_ADDRESS EEPROM_START
// EEPROM location of parent id
#define EEPROM_PARENT_NODE_ID_ADDRESS (EEPROM_START+1)
// EEPROM location of distance to gateway
#define EEPROM_DISTANCE_ADDRESS (EEPROM_PARENT_NODE_ID_ADDRESS+1)
#define EEPROM_ROUTES_ADDRESS (EEPROM_DISTANCE_ADDRESS+1) // Where to start storing routing information in EEPROM. Will allocate 256 bytes.
#define EEPROM_CONTROLLER_CONFIG_ADDRESS (EEPROM_ROUTES_ADDRESS+256) // Location of controller sent configuration (we allow one payload of config data from controller)
#define EEPROM_FIRMWARE_TYPE_ADDRESS (EEPROM_CONTROLLER_CONFIG_ADDRESS+24)
#define EEPROM_FIRMWARE_VERSION_ADDRESS (EEPROM_FIRMWARE_TYPE_ADDRESS+2)
#define EEPROM_FIRMWARE_BLOCKS_ADDRESS (EEPROM_FIRMWARE_VERSION_ADDRESS+2)
#define EEPROM_FIRMWARE_CRC_ADDRESS (EEPROM_FIRMWARE_BLOCKS_ADDRESS+2)
#define EEPROM_LOCAL_CONFIG_ADDRESS (EEPROM_FIRMWARE_CRC_ADDRESS+2) // First free address for sketch static configuration


int address = 0;
int distance;
byte value;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  // Node ID
  Serial.print("\tNode ID\t(");
  address = EEPROM_NODE_ID_ADDRESS;
  Serial.print(address);
  //  EEPROM.write(address, 3);
  Serial.print("):\t");
  value = EEPROM.read(address);
  Serial.print(value, DEC);
  Serial.print("\t0x");
  Serial.print(value, HEX);
  Serial.println();

  // Parent node ID
  Serial.print("Parent node ID\t(");
  address = EEPROM_PARENT_NODE_ID_ADDRESS;
  Serial.print(address);
  Serial.print("):\t");
  value = EEPROM.read(address);
  Serial.print(value, DEC);
  Serial.print("\t0x");
  Serial.print(value, HEX);
  Serial.println();

  // Distance
  Serial.print("Distance\t(");
  address = EEPROM_DISTANCE_ADDRESS;
  Serial.print(address);
  Serial.print("):\t");
  value = EEPROM.read(address);
  distance = value;
  Serial.print(value, DEC);
  Serial.print("\t0x");
  Serial.print(value, HEX);
  Serial.println();

  Serial.print("\tRoute\t(");
  address = EEPROM_ROUTES_ADDRESS;
  Serial.print(address);
  Serial.print("):\tme");
  value = EEPROM.read(address);
  if (distance == 255) {
    distance = 0;
  }
  for (int i = distance - 1; i >= 0; i--) {
    Serial.print(" => ");
    Serial.print(EEPROM.read(address + i), DEC);
  }
  Serial.println();

  Serial.print("Controller cfg\t(");
  address = EEPROM_CONTROLLER_CONFIG_ADDRESS;
  Serial.print(address);
  Serial.print("):\t");
  //  value = EEPROM.read(address);
  for (int i = 0; i < 256; i++) {
    int byte = EEPROM.read(address + i);
    if (byte != 0xFF) {
      Serial.print(i);
      Serial.print("=");
      Serial.print(byte, HEX);
      Serial.print(",");
    }
  }
  Serial.println();

  Serial.print("Firmware type\t(");
  address = EEPROM_FIRMWARE_TYPE_ADDRESS;
  Serial.print(address);
  Serial.print("):\t");
  //  value = EEPROM.read(address);
  for (int i = 0; i < 24; i++) {
    int byte = EEPROM.read(address + i);
    if (byte != 0xFF) {
      Serial.print(i);
      Serial.print("=");
      Serial.print(byte, HEX);
      Serial.print(",");
    }
  }
  Serial.println();


  Serial.print("Firmware ver\t(");
  address = EEPROM_FIRMWARE_VERSION_ADDRESS;
  Serial.print(address);
  Serial.print("):\t");
  value = EEPROM.read(address);
  Serial.print("0x");
  Serial.print(value, HEX);
  Serial.print(",0x");
  Serial.print(EEPROM.read(address + 1), HEX);
  Serial.println();

  Serial.print("Local cfg\t(");
  address = EEPROM_LOCAL_CONFIG_ADDRESS;
  Serial.print(address);
  Serial.print("):\t");
  value = EEPROM.read(address);
  Serial.print("0x");
  Serial.print(value, HEX);
  Serial.print(",0x");
  Serial.print(EEPROM.read(address + 1), HEX);
  Serial.println();


  //#define EEPROM_CONTROLLER_CONFIG_ADDRESS (EEPROM_ROUTES_ADDRESS+256) // Location of controller sent configuration (we allow one payload of config data from controller)
  //#define EEPROM_FIRMWARE_TYPE_ADDRESS (EEPROM_CONTROLLER_CONFIG_ADDRESS+24)
  //#define EEPROM_FIRMWARE_VERSION_ADDRESS (EEPROM_FIRMWARE_TYPE_ADDRESS+2)
  //#define EEPROM_FIRMWARE_BLOCKS_ADDRESS (EEPROM_FIRMWARE_VERSION_ADDRESS+2)
  //#define EEPROM_FIRMWARE_CRC_ADDRESS (EEPROM_FIRMWARE_BLOCKS_ADDRESS+2)
  //#define EEPROM_LOCAL_CONFIG_ADDRESS (EEPROM_FIRMWARE_CRC_ADDRESS+2) // First free address for sketch static configuration

  Serial.print("\n\n\n\n");



  // Shorter, useful for putting at the start of other sketches, don't forget the #include <EEPROM.h>

  // #include <EEPROM.h>
  Serial.print("Node ID: ");
  Serial.println(EEPROM.read(0));
  Serial.print("Parent node ID: ");
  Serial.println(EEPROM.read(1));
  Serial.print("Distance: ");
  Serial.println(EEPROM.read(2));
  Serial.print("Route : me");
  for (int i = EEPROM.read(2) - 1; i >= 0; i--) {
    Serial.print(" => ");
    Serial.print(EEPROM.read(3 + i), DEC);
  }



  Serial.println("\n\nfinished.");
  while (1);
}
