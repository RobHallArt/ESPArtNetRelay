// Adapted from https://github.com/rstephan/ArtnetWifi/tree/master/examples/ArtnetWifiDebug
// Runs on ESP8266 or ESP-01 and requires libraries.

#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>
#include <ArtnetWifi.h>

//Wifi settings
const char* ssid = "showAP";
const char* password = "4000HZ-AP";

// Set DMX channel which will trigger relay.
const int listenOnChannel = 0;

// Create Udp and ArtNet objects.
WiFiUDP UdpSend;
ArtnetWifi artnet;

// Connect to wifi – returns true if successful or false if not.
boolean ConnectWifi(void)
{
  boolean state = true;
  int i = 0;

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");
  
  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  // Print different messages to serial output depending on connection success.
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}

// Function to run every time a dmx packet is recieved.
void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  // For debugging print the values recieved on the specified channel to serial.
  Serial.print(" Value : ");
  Serial.print(data[listenOnChannel]);
  Serial.println();
  // Set state of relay based on channel value
  if(data[listenOnChannel]>128){
    digitalWrite(0, LOW);
  } else {
    digitalWrite(0, HIGH);
  }
  
}

void setup()
{
  // Setup serial for debug output
  Serial.begin(115200);
  ConnectWifi();
  // Set mode of the relay pin to OUTPUT to prevent serial signals being sent on it.
  pinMode(0,OUTPUT);

  // This will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
  artnet.begin();
}

void loop()
{
  // We call the read function inside the loop to check for new packets.
  artnet.read();
}
