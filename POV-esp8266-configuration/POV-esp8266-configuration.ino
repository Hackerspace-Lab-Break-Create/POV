#include <ESP8266WiFi.h>

#define NAME "MEO-739510"
#define PASS "fb5cd86358"

const char* host = "192.168.1.71";
const int pin = D1;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println();
pinMode(pin, OUTPUT);
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
}
void loop() {



  if (client.available()) {
   
    String data = client.readStringUntil('\n');
    Serial.print("POV message: ");
    Serial.println(data);
  }
  digitalWrite(pin, !digitalRead(pin));
delay(500);

}
