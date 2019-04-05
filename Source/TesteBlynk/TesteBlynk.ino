#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "425cb1306b604ab49e8897b29abd0f28";
char ssid[] = "TP-LINK_LAB-LSMO";
char pass[] = "sensores-lsmo";

BLYNK_CONNECTED(){
  Blynk.syncVirtual(V1);
}

void setup() {

  pinMode(2, OUTPUT);
  Blynk.begin(auth, ssid, pass);

}

void loop() {
  Blynk.run();
}

// fbassan testando o git