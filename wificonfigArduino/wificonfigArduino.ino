#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
const int RS = 10, EN = 11, D4 = 9, D5 = 8, D6 = 7, D7 = 6;
const int buttonChoose = 14;
const int buttonUp = 15;
const int buttonDown = 16;
String incoming;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
SoftwareSerial serial(2, 3);



void setup() {

  serial.begin(9600);


  pinMode(buttonChoose, INPUT);
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
//  lcd.autoscroll();

}

void loop() {



  char letter;

  if(digitalRead(buttonChoose) == HIGH){
  
      while (serial.available()) {
    letter = serial.read();
    incoming += letter;
  }
  lcd.print(incoming);
  }
  
  if(digitalRead(buttonUp) == HIGH){
    lcd.clear();
    lcd.setCursor(0,0);
    incoming = "";
  }
  
delay(500);

  }
