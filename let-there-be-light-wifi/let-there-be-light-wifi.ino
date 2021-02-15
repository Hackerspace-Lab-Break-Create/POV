#include "LedWord.h"
#include <ESP8266WiFi.h>
#include <ShiftRegister74HC595.h>
#include <arduino-timer.h>
#include <ArduinoWebsockets.h>

#define NAME "MEO-739510"
#define PASS "fb5cd86358"
using namespace websockets;

ShiftRegister74HC595<1> sr (D0, D1, D2);

WebsocketsClient client;

auto timer = timer_create_default();

const int VIEW_SIZE = 16;
const int BUFFER_LENGTH = VIEW_SIZE * 8;
const char* host = "192.168.1.71";

float roundTripTime = .01; //// Time (Hz) for 1 cycle. 1 complete buffer push

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

  String c = "Insert Word";
  char bufferA[c.length()];
  c.toCharArray(bufferA, c.length());
  ledWord.setWord(bufferA);

  // timer.every(10000, checkHost);
  timer.every(500, toggle_leds);
  timer.every(10000, checkMessage);

}

void loop()
{
  timer.tick();

  if (client.available()) {
    client.poll();
  }

}
