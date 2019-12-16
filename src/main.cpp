#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Update.h>
#include <elapsedMillis.h>
#include <ota.h>
#include "kaaro_utils.cpp"
// #include "neopixel_utils.cpp"
#include <Adafruit_NeoPixel.h>
#include<Preferences.h>
/* 
    STATICS
*/
#define LED_PIN    15
#define LED_COUNT  380
const char *mqtt_server = "api.akriya.co.in";

/* 
    FUNCTION DEFINATIONS
*/


void mqttCallback(char *topic, byte *payload, unsigned int length);

   
/* 
 REALTIME VARIABLES
*/

String host = "ytkarta.s3.ap-south-1.amazonaws.com"; // Host => bucket-name.s3.region.amazonaws.com
int port = 80;                                       // Non https. For HTTPS 443. As of today, HTTPS doesn't work.
String bin = "/kaaroMerch/SubsCount/firmware.bin";   // bin file name with a slash in front.

bool isWificonnected;
char mo[75];
String msg = "";
String DEVICE_MAC_ADDRESS;
String ssid = "";
String pass = "";
byte mac[6];

String wifiStatus;
unsigned long delayStart = 0; // the time the delay started
bool delayRunning = false;
unsigned int interval = 2500;



/*
  HY Variable/Instance creation
*/
Preferences preferences;
WiFiClient wifiClient;
PubSubClient mqttClient(mqtt_server, 1883, mqttCallback, wifiClient);
WiFiManager wifiManager;
elapsedMillis timeElapsed;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);


void mqttCallback(char *topic, uint8_t *payload, unsigned int length)
{
  char *cleanPayload = (char *)malloc(length + 1);
  payload[length] = '\0';
  memcpy(cleanPayload, payload, length + 1);
  msg = String(cleanPayload);
  free(cleanPayload);

  String topics = String(topic);
  Serial.print("From MQTT = ");
  Serial.println(msg);

  if (topics == "tajOta/default")
  {
    if (msg.startsWith("/")){
    Serial.printf("Ota Initiating from link : %s", msg);

    OTA_ESP32::execOTA(host, port, msg, &wifiClient);
    }
    else if(msg.equals("default")){
    Serial.printf("Ota Initiating from Default link : %s", bin);

    OTA_ESP32::execOTA(host, port, bin, &wifiClient);
    }

    if(msg.equals("G")||msg.equals("g")){
  for(int j = 0; j < 3; j++) {

 for(int i=0; i<LED_COUNT; i++) { // For each pixel...
    strip.setPixelColor(i, strip.Color(0, 255, 0,0));
    delay(1); // Pause before next pass through loop
  }
      strip.show();   // Send the updated pixel colors to the hardware.
    delay(100);
    for(int i=0; i<LED_COUNT; i++) { // For each pixel...
    strip.setPixelColor(i, strip.Color(0, 0, 0,0));
    delay(1); // Pause before next pass through loop
  }   
    strip.show();   // Send the updated pixel colors to the hardware.
  }
    for(int i=0; i<LED_COUNT; i++) { // For each pixel...
    strip.setPixelColor(i, strip.Color(0, 0,0,255));
    delay(1); // Pause before next pass through loop
       // Send the updated pixel colors to the hardware.

  } 
  strip.show();
  }
      if(msg.equals("R")|| msg.equals("r")){
  for(int j = 0; j < 3; j++) {

    // strip.setPixelColor(0,255,0,0);
    for(int i=0; i<LED_COUNT; i++) { // For each pixel...
    strip.setPixelColor(i, strip.Color(255, 0, 0,0));
       // Send the updated pixel colors to the hardware.
    delay(1); // Pause before next pass through loop
  }
  strip.show();
    delay(100);
    for(int i=0; i<LED_COUNT; i++) { // For each pixel...
    strip.setPixelColor(i, strip.Color(0, 0, 0,0));
    delay(1); // Pause before next pass through loop
  }
  strip.show();   
  }
    for(int i=0; i<LED_COUNT; i++) { // For each pixel...
    strip.setPixelColor(i, strip.Color(0, 0,0,255));
    delay(1); // Pause before next pass through loop
  }
      strip.show();   // Send the updated pixel colors to the hardware.
      
}
    if(msg.equals("Y")||msg.equals("y")){
    for(int i=0; i<LED_COUNT; i++) { // For each pixel...
    strip.setPixelColor(i, strip.Color(255, 255, 0,0));
    delay(1); // Pause before next pass through loop
  }
      strip.show();   // Send the updated pixel colors to the hardware.
    }
    if(msg.equals("W")||msg.equals("w")){
    for(int i=0; i<LED_COUNT; i++) { // For each pixel...
    strip.setPixelColor(i, strip.Color(0, 0, 0,255));
    delay(1); // Pause before next pass through loop
  }
      strip.show();   // Send the updated pixel colors to the hardware.
    }

  }
  // else if (topics == "elle/leds"){
    
  
  // else if(topics = "elle/rgb")

}
  

void reconnect()
{
int i=0;
if(WiFi.status() == WL_CONNECTED){
  if (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");

    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str()))
    {
      Serial.println("connected");

      String readyTopic = "tajOta/" + DEVICE_MAC_ADDRESS;
      mqttClient.publish(readyTopic.c_str(), "Ready!");
      mqttClient.publish("tajOta", "Ready!");
      mqttClient.subscribe("tajOta/default");
      String otaTopic = "tajOta/" + DEVICE_MAC_ADDRESS;
      mqttClient.subscribe(otaTopic.c_str());
    }

    else if (WiFi.status() != WL_CONNECTED)
    {
      Serial.print("from here");
      while (WiFi.status() != WL_CONNECTED)
    {
        WiFi.mode(WIFI_STA);
    WiFi.disconnect();
 
    delay(100);
       WiFi.begin("wendor");
       Serial.print("Attempting to connect to  network, SSID: ");
        delay(5000);
 
       Serial.println("IP address: ");
       Serial.println(WiFi.localIP());
 
        wifiStatus = WiFi.status() == WL_CONNECTED;
    }
    }
  }
}
}
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
void setup()
{

  Serial.begin(115200);
  // preferences.begin("wifiKeys", false);

    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    strip.setBrightness(90); // Set BRIGHTNESS to about 1/5 (max = 255)
    // strip.Color(0,0,0,255);
    // strip.show();
      for(int i=0; i<LED_COUNT; i++) { // For each pixel...
    strip.setPixelColor(i, strip.Color(0, 0, 0,255));
    delay(1); // Pause before next pass through loop
  }
      strip.show();



  // neopixel_utils::initNeopixel();
  DEVICE_MAC_ADDRESS = KaaroUtils::getMacAddress();
  Serial.println(DEVICE_MAC_ADDRESS);
  WiFi.macAddress(mac);
    Serial.print("MAC: ");
    Serial.print(mac[0],HEX);
    Serial.print(":");
    Serial.print(mac[1],HEX);
    Serial.print(":");
    Serial.print(mac[2],HEX);
    Serial.print(":");
    Serial.print(mac[3],HEX);
    Serial.print(":");
    Serial.print(mac[4],HEX);
    Serial.print(":");
    Serial.println(mac[5],HEX);


  Serial.print("Connecting Wifi: ");
     WiFi.begin("wendor");
int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        if(i>10){
          break;
        }
        i++;
        Serial.print(".");

    }

  // String PD, SD;
  // if (preferences.getString("ssid","noKey")=="noKey"){
  // Serial.println("Not Having Wifi Keys");
  // wifiManager.setConnectTimeout(5);
  // wifiManager.setConfigPortalBlocking(false);
  // wifiManager.setWiFiAutoReconnect(true);
  // wifiManager.autoConnect("Taj-OTA");
  // if(wifiManager.autoConnect("Taj-OTA")){
  //   Serial.println("Autoconnect Successfull");
  //   // SD = preferences.getString("ssid");
  //   //  PD = preferences.getString("pass");
  // }
  // // }
  // else
  // {
  // //   SD = preferences.getString("ssid");
  // //   PD = preferences.getString("pass");
  //   Serial.printf("SSID is = wendor");
  //   WiFi.begin("wendor");
  // }
  


  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);
  isWificonnected = true;

    ssid = WiFi.SSID();
    pass = WiFi.psk();
    // preferences.putString("ssid", ssid);
    // preferences.putString("pwd",pass);  
  }


  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(mqttCallback);
}

void loop()
{

  wifiManager.process();

  //    if (timeElapsed > interval) 
  // {				
  //     Serial.print("From here");
  //   // display.showCustomMessage(" Total ");
  //     // neopixel_utils::colorWipe(strip.Color(255,   0,   0), 50);
  //     theaterChaseRainbow(50);
  //     // rainbow(10);             // Flowing rainbow cycle along the whole strip

  //   timeElapsed = 0;
  // }
  while (WiFi.status() != WL_CONNECTED)
    {
        WiFi.mode(WIFI_STA);
    WiFi.disconnect();
 
    delay(100);
       WiFi.begin("wendor");
       Serial.print("Attempting to connect to  network, SSID: ");
        delay(5000);
                // Serial.println(ssid);
 
       Serial.println("IP address: ");
       Serial.println(WiFi.localIP());
 
        wifiStatus = WiFi.status() == WL_CONNECTED;
 
    }

  if (WiFi.status() == WL_CONNECTED)
  {
      if (!mqttClient.connected()) {
    reconnect();
  }

  }
    mqttClient.loop();
}
