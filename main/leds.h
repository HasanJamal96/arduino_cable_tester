#include "Arduino.h"


class Leds {
  public:
    void initializeLeds() {
      pinMode(LED_RED, OUTPUT);
      pinMode(LED_GREEN, OUTPUT);
      pinMode(LED_AMBER, OUTPUT);
    }
    void showResult() {
      _closeAllLeds();
      switch(preResult) {
        case POSITIVE:
          if(curResult == NEGATIVE) {
            digitalWrite(LED_AMBER, HIGH);
          }
          else {
            digitalWrite(LED_RED, HIGH);
          }
          break;
        default:
          if(curResult == NEGATIVE) {
            digitalWrite(LED_GREEN, HIGH);
          }
          else {
            digitalWrite(LED_RED, HIGH);
          }
          break;
      }
      preResult = curResult;
    }

    void resetResult() {
      _closeAllLeds();
    }
  private:
    void _closeAllLeds() {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_AMBER, LOW);
    }
};
