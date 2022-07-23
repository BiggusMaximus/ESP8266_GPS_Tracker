<<<<<<< HEAD
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
SoftwareSerial serial_gps(D2, D1); 
TinyGPSPlus gps;
float latitude, longitude;

void setup() {
  Serial.begin(9600);
  serial_gps.begin(9600);

}

void loop() {
  
  while(serial_gps.available()) {
    gps.encode(serial_gps.read());
  }
  if(gps.location.isUpdated()) {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    String link = "www.google.com/maps/place/" + String(latitude) + "," + String(longitude) ;
    Serial.println(link);
    delay(500);
  }
}
