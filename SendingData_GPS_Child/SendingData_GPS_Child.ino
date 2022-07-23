<<<<<<< HEAD
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

SoftwareSerial serial_gps(D2, D1); 
TinyGPSPlus gps;
float latitude, longitude;
String link;
uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0x6A, 0xA4, 0x69};

typedef struct struct_message {
  float nilai_lat, nilai_lot;
  String gps_link;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 1000;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup() {
  Serial.begin(115200);
  serial_gps.begin(9600);
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  
  while(serial_gps.available()) {
    gps.encode(serial_gps.read());
  }
  if(gps.location.isUpdated()) {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    link = "www.google.com/maps/place/" + String(latitude) + "," + String(longitude) ;
    Serial.println(link);
    delay(500);
  }
  if ((millis() - lastTime) > timerDelay) {
    myData.nilai_lat = latitude;
    myData.nilai_long = longitude;
    myData.gps_link = link;
    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    lastTime = millis();
  }
}
