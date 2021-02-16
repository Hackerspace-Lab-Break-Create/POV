#include <ESP8266WiFi.h>
#include <ArduinoWebsockets.h>
#include <ArduinoOTA.h>

#define NAME "*****"
#define PASS "*****"
using namespace websockets;
const char* host = "******";

WebsocketsClient client;

void setup() {

  Serial.begin(115200);
  Serial.println();

  /* Set Client up as station */
  WiFi.mode(WIFI_STA);

  WiFi.begin(NAME, PASS);

  /* Connect to the network */
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  if (client.connect(host, 8080, "/povdisplay/message")) {
    Serial.print("Connected to: ");
    Serial.println(host);
  }

  ArduinoOTA.setHostname("*****");
  ArduinoOTA.setPassword("*****");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready");

  client.onMessage([&](WebsocketsMessage message) {
    Serial.print("POV message: ");
    Serial.println(message.data());
  });
}
void loop() {

  ArduinoOTA.handle();

  if (client.available()) {

    client.poll();
    client.send("message");
  }

  delay(500);

}
