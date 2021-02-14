
#ifndef _LED_WORD_
#define _LED_WORD_
#include "PovAlphabet8x8.h"
#include <Vector.h>


const int ELEMENT_COUNT_MAX = 50;
using namespace std;
class LedWord {

  public:

    const static int letterHeight = 8;
    int letterWidth = 8;

    byte* storage_array[ELEMENT_COUNT_MAX];
    Vector<byte*> letters;

    LedWord () {
      letters.setStorage(storage_array);
    }

    byte nextCol();
    void setWord(char *word);
    void setLetter (int letterIndex, char letter);
    int length () {
      return letters.size();
    }

  private:
    int currentIndex = 7;
    // The letters are read as normal from left to right
    int currentLetter = 0;
    byte ret;

};

void LedWord::setLetter (int letterIndex, char letter) {
  char index = letter;


  // Force uppercase
  if (letter >= 97 && letter <= 122) {
    index = letter - 32;
  } else {
    index = letter;
  }

  //Check for special characters
  switch (letter) {
    case '$':
      index = 27 + 65;
      break;

    case ' ':
      index = 26 + 65;
      break;
  }


  letters.push_back(ALPHABET[(index - 65)]);

}

void LedWord::setWord (char *newWord) {
  letters.clear();
  currentIndex = 7;
  currentLetter = 0;
  int i = 0;
  while (*newWord != '\0') {
    setLetter(i++, *newWord);
    newWord++;
  }

}

byte LedWord::nextCol () {

  byte *letter = letters[currentLetter];

  for (int i = 0; i < letterHeight; i++) {
    bitWrite(ret, i, bitRead(letter[i], currentIndex));
  }

  currentIndex--;

  if (currentIndex < 0) {
    currentIndex = 7;

    currentLetter++;

    if (currentLetter >= letters.size()) {
      currentLetter = 0;
    }
  }
  return ret;
}
#endif
