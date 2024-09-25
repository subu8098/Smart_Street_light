#include <ESP32Firebase.h>
#include <TinyGPS++.h>

#define GPS_RX_PIN 16
#define GPS_TX_PIN 17
#define LDR 34
#define _SSID "vkprojects"          // Your WiFi SSID
#define _PASSWORD "vkprojects"      // Your WiFi Password
#define REFERENCE_URL "https://iotled-3786f-default-rtdb.firebaseio.com/"  // Your Firebase project reference url

Firebase firebase(REFERENCE_URL);
HardwareSerial gpsSerial(1); // Use hardware serial port 1

TinyGPSPlus gps;
void setup() {
  
  Serial.begin(9600);
  //connect to wifi
 connectToWiFi();
//pinmode
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

pinMode(LDR, INPUT);

  
}

void loop() {

//firebase.deleteData("Example");
float ldr_value=analogRead(LDR);
Serial.println(ldr_value); 
if(ldr_value>1500)
{
  Serial.println(ldr_value);
  delay(2000);
}

if(ldr_value<1500)
{
 while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        double lat=gps.location.lat();
        String latitude=String(lat,6);
        double lon=gps.location.lng();
        String longitude=String(lon,6);
        Serial.println("light intensity is low,and the location is:");
        Serial.print("Latitude: ");
        Serial.println(lat,6);
        Serial.print("Longitude: ");
        Serial.println(lon,6);
        firebase.pushString("location","latitude");
        firebase.pushString("location",latitude);
        firebase.pushString("location","longitude");
        firebase.pushString("location",longitude);
        // Serial.print("Latitude: ");
        // Serial.print(gps.location.lat(), 6);
        // Serial.print(", Longitude: ");
        // Serial.println(gps.location.lng(), 6);
        delay(2000);
      } else {
        Serial.println("Location data invalid.");
      }
    }
  }
  firebase.pushString("Warning","Light's intensity is low in light 1,Please check!!!");

  firebase.pushString("Warning","Value:");
  firebase.pushFloat("Warning",ldr_value);
  
 
delay(2000);
}
  

}
void connectToWiFi(){
WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  }
