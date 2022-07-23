#include <ESP8266WiFi.h>
#include <espnow.h>

float latitude, longitude;
String link;

typedef struct struct_message {
  float nilai_lat, nilai_lot;
  String gps_link;
} struct_message;

struct_message myData;
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  longitude   = myData.nilai_lot;
  latitude    = myData.nilai_lat;
  link        = myData.gps_link;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(
    "Latitude : " + String(latitude) + 
    " | Longitude " + String(longitude) +
    " | GPS : " + String(link));
    
}
