#include <ESP8266WiFi.h>
#include <ArduinoWebsockets.h>

#define NAME "MEO-739510"
#define PASS "fb5cd86358"
using namespace websockets;
const char* host = "192.168.1.71";

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

  client.onMessage([&](WebsocketsMessage message){
     Serial.print("POV message: ");
    Serial.println(message.data());
  });
}
void loop() {



  if (client.available()) {
   
    client.poll();
   client.send("message");
  }
 
delay(500);

}
