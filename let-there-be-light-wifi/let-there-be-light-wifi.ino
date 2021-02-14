#include "LedWord.h"
#include <ESP8266WiFi.h>
#include <ShiftRegister74HC595.h>
#include <arduino-timer.h>

#define NAME "******"
#define PASS "******"

ShiftRegister74HC595<1> sr (D0, D1, D2);
WiFiClient client;
auto timer = timer_create_default();

const int VIEW_SIZE = 16;
const int BUFFER_LENGTH = VIEW_SIZE * 8;
const char* host = "******";

float roundTripTime = .01; //// Time (Hz) for 1 cycle. 1 complete buffer push

LedWord ledWord;
boolean toggle = true;
byte values;

bool toggle_leds(void *) {

  values = ledWord.nextCol();
  for (int i = 0; i < 8; i++) {
    sr.set(7 - i, bitRead(values, i) == 0 ? LOW : HIGH);
  }

  return true;
}

bool checkHost(void *) {
  if(client.available()){
    return true;  
  }
  if (client.connect(host, 9090)) {
    Serial.print("Connected to: ");
    Serial.println(host);
  }
  return true;
}

void setup()
{

  Serial.begin(115200);

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

  if (client.connect(host, 9090)) {
    Serial.print("Connected to: ");
    Serial.println(host);
  }

  String c = "Insert Word";
  char bufferA[c.length()];
  c.toCharArray(bufferA, c.length());
  ledWord.setWord(bufferA);

 // timer.every(10000, checkHost);
  timer.every(5, toggle_leds);

}

void loop()
{
timer.tick();

 if (client.available()) {

    String c = client.readString();
    char bufferA[c.length()];
    c.toCharArray(bufferA, c.length());
    ledWord.setWord(bufferA);
  }

}
