#include "LedWord.h"
#include <ESP8266WiFi.h>
#include <ShiftRegister74HC595.h>
#include <arduino-timer.h>
#include <ArduinoWebsockets.h>
#include <ArduinoOTA.h>

#define NAME "*******"
#define PASS "*******"
using namespace websockets;

ShiftRegister74HC595<1> sr (D0, D1, D2);

WebsocketsClient client;

auto timer = timer_create_default();

const char* host = "*******";

LedWord ledWord;
boolean toggle = true;
byte values;
String toDisplay;

bool toggle_leds(void *) {

  values = ledWord.nextCol();
  for (int i = 0; i < 8; i++) {
    sr.set(7 - i, bitRead(values, i) == 0 ? LOW : HIGH);
  }

  return true;
}

bool checkMessage(void *) {
  client.send("message");
  return true;
}

void setup()
{

  // "clock" led so we can see each pulse of the time step
  pinMode (LED_BUILTIN, OUTPUT);

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

  ArduinoOTA.setHostname("******");
  ArduinoOTA.setPassword("******");

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

    if (message.data() == toDisplay){
      
    } else{
    String c = message.data();
    toDisplay = c;
    char bufferA[c.length()];
    c.toCharArray(bufferA, c.length());
    ledWord.setWord(bufferA);
    }
  });

 
  timer.every(50, toggle_leds);
  timer.every(10000, checkMessage);

}

void loop()
{
  ArduinoOTA.handle();
  timer.tick();

  if (client.available()) {
    client.poll();
  }

}
