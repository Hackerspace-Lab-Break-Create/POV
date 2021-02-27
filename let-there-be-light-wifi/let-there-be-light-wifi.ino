#include "LedWord.h"
#include <ESP8266WiFi.h>
#include <ShiftRegister74HC595.h>
#include <arduino-timer.h>
#include <ArduinoWebsockets.h>
#include <ArduinoOTA.h>
#include "ColDisplay.h"

#define NAME "MEO-739510"
#define PASS "fb5cd86358"
using namespace websockets;

ShiftRegister74HC595<1> sr (D0, D2 , D3);
ShiftRegister74HC595<1> sr2 (D1, D5 , D6);

WebsocketsClient client;

auto timer = timer_create_default();

const char* host = "192.168.1.71";

LedWord ledWord;
ColDisplay colDisplay;
boolean toggle = true;
byte values;
String toDisplay;
int counter = 0;
byte letter[8][2];
byte* letterfirst;
byte* lettersecond;
const int pingPin = D8;
const int echoPin = D7;
boolean turn = true;
int counter2 = 10;


void toggle_leds() {

  for (int c = 0; c < ledWord.length() * 8; c++) {
    values = ledWord.nextCol();
    for (int i = 0; i < 8; i++) {
      sr.set(i, bitRead(values, i) == 0 ? LOW : HIGH);
    }
    delay(2);
  }

}

void letterWord() {

  letterfirst = ledWord.letters[counter];
  lettersecond = ledWord.letters[counter + 1];

  for (int j = 0; j < 8; j++) {

    letter[j][0] = letterfirst[j];

    letter[j][1] = lettersecond[j];
  }

  colDisplay.fillBuffer(0b00000000);
  colDisplay.setBytes (counter2, 0, (byte*)letter, 8, 2);
  
  for (int i = 0; i < 8; i++) {
 
    for (int j = 0; j < 4; j++) {
      for (int k = 1; k <= 8; k++) {
        sr.set(k-1, bitRead(colDisplay.displayBuffer[i][j], 8 - k));
      }
delay(2);
    }
  }

  counter2--;
  if (counter2 == 0) {
    counter++;
    counter2 = 10;
  }

  if (counter >= toDisplay.length() - 2) {
    counter = 0;
    colDisplay.setPixel (0, 0, true);
    
    for (int i = 0; i < 8; i++) {
   
      for (int j = 0; j < 4; j++) {
        for (int k = 1; k <= 8; k++) {
          sr.set(k-1, bitRead(colDisplay.displayBuffer[i][j], 8 - k));
        }

      }
    }
  }
}

bool checkMessage(void *) {
  client.send("message");
  return true;
}

void setup()
{

  for (int i = 0; i < 8; i++) {
    sr2.set(i, LOW);
  }
  // "clock" led so we can see each pulse of the time step
  pinMode (LED_BUILTIN, OUTPUT);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);

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

  ArduinoOTA.setHostname("espTofu");
  ArduinoOTA.setPassword("kali");

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

    if (message.data() == toDisplay) {

    } else {
      String c = message.data();
      toDisplay = c;
      char bufferA[c.length()];
      c.toCharArray(bufferA, c.length());
      ledWord.setWord(bufferA);
    }
  });


  //timer.every(5, letterWord);
  //timer.every(2, toggle_leds);
  timer.every(10000, checkMessage);

}

void loop()
{

  ArduinoOTA.handle();
  timer.tick();


  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  int duration = pulseIn(echoPin, HIGH);
  if (duration < 500 && turn) {

    letterWord();
    turn = false;
  }

  if ( duration > 1000) {
    turn = true;
  }
  if (client.available()) {
    client.poll();
  }

}
