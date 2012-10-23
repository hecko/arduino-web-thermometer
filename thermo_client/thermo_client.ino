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
byte gateway[] = { 
  192, 168, 88, 1 };
byte server[] = { 
  178, 79, 144, 114 };

int relay_port = 3;

//define the server on which the temperature database resides
EthernetClient client;

OneWire oneWire(2);

DallasTemperature sensors(&oneWire);
DeviceAddress Thermometer;
int numberOfThermometers;

String connectAndRead() {
  Serial.println("Connecting to decision server...");
  //connect to server and return read value
  if (client.connect(server, 80)) {
    client.println("GET /maco/home/decide.php HTTP/1.0");
    client.println();
    delay(100);
    return readPage();
  }
}

char inString[32]; // string for incoming serial data
int stringPos = 0; // string index counter
boolean startRead = false; // is reading?

String readPage(){
  //read the page, and capture & return everything between '<' and '>'

  stringPos = 0;
  memset( &inString, 0, 32 ); //clear inString memory

  while(true){

    if (client.available()) {
      char c = client.read();

      if (c == '<' ) { //'<' is our begining character
        startRead = true; //Ready to start reading the part 
      }
      else if(startRead){

        if(c != '>'){ //'>' is our ending character
          inString[stringPos] = c;
          stringPos ++;
        }
        else{
          //got what we need here! We can disconnect now
          startRead = false;
          client.stop();
          client.flush();
          Serial.println("disconnecting.");
          return inString;

        }

      }
    }

  }

}

void setup(void) {
  delay(5000); //allow slack time for uploading differenet sketch
  Ethernet.begin(mac,ip,gateway);
  Serial.begin(9600);
  sensors.begin();
  numberOfThermometers = sensors.getDeviceCount();
  sensors.getAddress(Thermometer,0);
  sensors.setResolution(Thermometer, 12);
  pinMode(relay_port, OUTPUT); //for relay
  digitalWrite(relay_port, LOW);
}

void loop(void)
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(Thermometer);

  //delay(100);
  Serial.println("Connecting to send data...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET ~/maco/home/in/?key=");
    for (uint8_t i = 0; i < 8; i++) {
      if (Thermometer[i] < 16) client.print("0");
      client.print(Thermometer[i], HEX);
    }
  }

  client.print("&val=");
  client.print(tempC);
  client.println(" HTTP/1.0");
  client.println();
  Serial.println(tempC);

  client.stop();

  delay(500);
  //connect to the server, request whether to turn the relay ON or left OFF
  String d = connectAndRead();
  Serial.print("Decision: ");
  Serial.println(d);
  
  if (d == "1") {
    digitalWrite(relay_port, HIGH);
  } else {
    digitalWrite(relay_port, LOW);
  }
  d = "";

  //1000 = 1 second
  delay(60000);
  //delay(1000);

}


