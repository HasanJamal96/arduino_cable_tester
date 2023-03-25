#include "Arduino.h"


/*
If any pins do not have the expected
continuity, the red light is lit and the amber
and green lights are off. 

The bottom line of the 2 line lcd displays In x Out X error, where x is
the respective input and output pins that have errors. With more than one error the display
should display the errors in sequence with a 1 second delay between each display.

If the pins have the expected continuity, but previously they have been in the red state,
the amber light is lit and the green and red lights are off. The display should show In x
out x Intermittent, where x is the respective
input and output pins that have errors. With more than one error the display should display
the errors in sequence with a 1 second delay
between each display.
Pressing the "test" button clears any errors
and the display of the same until such time
another error is detected.
*/

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
        case POSITIVE: // Error
          if(curResult == NEGATIVE) { // no Error
            digitalWrite(LED_AMBER, HIGH);
          }
          else {
            digitalWrite(LED_RED, HIGH);
          }
          break;
        default: // None or No Error previously
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
