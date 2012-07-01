#include <SPI.h>
#include <Ethernet.h>
#include <DallasTemperature.h>
#include <OneWire.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 
  192, 168, 88, 100 };
byte subnet[] = { 
  255, 255, 255, 0 };
byte gateway[] = { 
  192, 168, 88, 1 };
byte server[] = { 
  178, 79, 144, 114 };

//define the server on which the temperature database resides
EthernetClient client;

//OneWire ds(2);

//DallasTemperature sensors(&oneWire);
//DeviceAddress Thermometer;
//int numberOfThermometers;


void setup(void) {
  Ethernet.begin(mac,ip,gateway,subnet);
  Serial.begin(9600);
  //sensors.begin();
  //numberOfThermometers = sensors.getDeviceCount();
  //sensors.getAddress(Thermometer,0);
  //sensors.setResolution(Thermometer, 12);
}

void loop(void)
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  //sensors.requestTemperatures();
  //float tempC = sensors.getTempC(Thermometer);

  //delay(100);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET /home/in/?key=");
    //for (uint8_t i = 0; i < 8; i++) {
    //  if (Thermometer[i] < 16) client.print("0");
    //  client.print(Thermometer[i], HEX);
  }

  client.print("&val=");
  //client.print(tempC);
  client.println(" HTTP/1.0");
  client.println();


  Serial.println("...");


  client.stop();
  delay(60000);

}


