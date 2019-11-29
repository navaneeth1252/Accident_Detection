#include <ESP8266WiFi.h>
#include "IFTTTWebhook.h"
#include <SoftwareSerial.h> 
#include <TinyGPS++.h>
void getlocation();
long duration;
int distance;
char lati[10];
char lon[10];
double x,y;
TinyGPSPlus gps;
SoftwareSerial SoftSerial(D7,D8); // Receiver and transmitter pins for GPS module respectively 
#define WIFI_SSID "xxxxxxxx" //Wi-fi user id
#define  WIFI_PASSWORD "xxxxxx" //Wi-fi password
const int vibrator = D0;

#define IFTTT_API_KEY "xxxxxxxxxxxxxxxxxxxxxxx"  //API key of IFTTT platform i.e webhooks
#define IFTTT_EVENT_NAME "xxxxxxx"  //Event name present in the creation of webhooks
#define IFTTT_FINGERPRINT "AA:75:CB:41:2E:D5:F9:97:FF:5D:A0:8B:7D:AC:12:21:08:4B:00:8C" //Default for the server

void setup() {
  SoftSerial.begin(9600); 
  Serial.begin(9600);
  Serial.println("RUNNING");
  pinMode(vibrator, INPUT);
  //Connecting to the wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}

void loop() {
  getlocation();
  Serial.println(digitalRead(vibrator));
  if(digitalRead(vibrator)==1)//reads the values from the vibrator 
  {
      Serial.println(lati);
      Serial.println(lon);
      IFTTTWebhook wh(IFTTT_API_KEY, IFTTT_EVENT_NAME,IFTTT_FINGERPRINT);
      wh.trigger(lati,lon);//triggers the event with latitude and longitude
      Serial.println("Triggered");
  }
}
//Code for getting the location of the accident
void getlocation()
{
  while (SoftSerial.available() > 0)
   gps.encode(SoftSerial.read());
   if (gps.location.isUpdated())
    {
      x=gps.location.lat();
      y=gps.location.lng();
      Serial.print("LAT="); Serial.print(gps.location.lat(), 6);
      Serial.print("  LNG="); Serial.println(gps.location.lng(), 6);
    }
}
