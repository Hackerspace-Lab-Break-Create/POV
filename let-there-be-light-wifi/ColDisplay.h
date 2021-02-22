#ifndef __ROW_DISPLAY_H__
#define __ROW_DISPLAY_H__

#include <Vector.h>

const static int MAX_WIDTH = 4;  //BYTES
const static int MAX_HEIGHT = 8; //LEDS

class ColDisplay {
  private:

    byte displayBuffer[MAX_HEIGHT][MAX_WIDTH];
    int pixelHeight = MAX_HEIGHT; //Number of leds verticaly
    int viewWidthNumBytes = MAX_WIDTH; //Number of bytes in view

  public:
    byte values;

    ColDisplay () {
      fillBuffer(0b00000000);
    }

    void updateLoop ();
    void printDisplay();
    void setPixel (int x, int y, bool val);
    void fillBuffer (byte val);
    void fillView ();
    void setBytes (int x, int y, byte* val, int height, int width);
};


void ColDisplay::setBytes (int x, int y, byte* val, int height, int width) {
  if (x < 0 || (x / 8) > MAX_WIDTH || y < 0 || y > MAX_HEIGHT - 1) {
    return;
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {

      byte current = *((val + i * width) + j);

      for (int m = 0; m < 8; m++) {
        int xa = ((x + j) + m + j * 8) + x;
        if ( (xa / 8) > MAX_WIDTH - 1) {
          break;
        }
        int ya = y + i;
        setPixel (xa, ya, bitRead(current, 7 - m));
      }

    }

  }



}

void ColDisplay::fillBuffer (byte val) {
  for (int i = 0; i < MAX_HEIGHT; i++) {
    for (int j = 0; j < MAX_WIDTH; j++) {
      displayBuffer[i][j] = val;
    }
  }

}
void ColDisplay::setPixel (int x, int y, bool val) {
  if ( x < 0 || (x / 8) > MAX_WIDTH || y < 0 || y > MAX_HEIGHT - 1) {
    return;
  }
  bitWrite(displayBuffer[y][(x / 8)], (8 - (x % 8)) - 1, val);
}

void ColDisplay::printDisplay () {
  Serial.println("");
  for (int i = 0; i < pixelHeight; i++) {
    Serial.println("");
    for (int j = 0; j < viewWidthNumBytes; j++) {
      for (int k = 1; k <= 8; k++) {
        Serial.print(bitRead(displayBuffer[i][j], 8 - k));     
      }
   
    }
  }
}


void ColDisplay::updateLoop() {

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

}

#endif
