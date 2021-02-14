#include "LedWord.h"
#include <ESP8266WiFi.h>
#include <ShiftRegister74HC595.h>

#define NAME "MEO-739510"
#define PASS "fb5cd86358"

ShiftRegister74HC595<1> sr (D0, D1, D2);
WiFiClient client;

const int VIEW_SIZE = 16;
const int BUFFER_LENGTH = VIEW_SIZE * 8;
const char* host = "192.168.1.71";

float roundTripTime = .01; //// Time (Hz) for 1 cycle. 1 complete buffer push

LedWord ledWord;
boolean toggle = true;
byte values;

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
}

void loop()
{

  if (client.available()) {

    String c = client.readString();
    char bufferA[c.length()];
    c.toCharArray(bufferA, c.length());
    ledWord.setWord(bufferA);
  }

  //Implement interrupt herew
  values = ledWord.nextCol();

  for (int i = 0; i < 8; i++) {
    sr.set(7 - i, bitRead(values, i) == 0 ? LOW : HIGH);
  }


  delay(500);

}
