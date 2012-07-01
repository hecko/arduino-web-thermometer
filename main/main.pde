#include <SPI.h>
#include <Ethernet.h>
#include "DallasTemperature.h"
#include "OneWire.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 
  192,168,88,100 };
byte gateway[] = {
  192,168,88,1
};
//define the server on which the temperature database resides
byte server[] = { 
  170,79,144,114 };

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
Client client(server, 80);

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometer;
int numberOfThermometers;


void setup() {
  // start the Ethernet connection:
  //Ethernet.begin(mac, ip);
  Ethernet.begin(mac,ip,gateway);
  // start the serial library:
  Serial.begin(9600);
  // give the Ethernet shield a second to initialize:
  sensors.begin();
  numberOfThermometers = sensors.getDeviceCount();
  sensors.getAddress(Thermometer,0);
  sensors.setResolution(Thermometer, 12);
}

void loop()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(Thermometer);

  delay(100);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect()) {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET /home/in/?key=");
    for (uint8_t i = 0; i < 8; i++) {
      if (Thermometer[i] < 16) client.print("0");
      client.print(Thermometer[i], HEX);
    }

    client.print("&val=");
    client.print(tempC);
    client.println(" HTTP/1.0");
    client.println();
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }

  client.stop();
  delay(60000);

}



