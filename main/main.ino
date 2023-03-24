#include "config.h"
#include "ArduinoJson.h"
#include <LiquidCrystal_I2C.h>


StaticJsonDocument <512> doc;

LiquidCrystal_I2C lcd(address, cols, rows);


results_t     curResult = NONE;  // current result
results_t     preResult = NONE;  // previous result
test_state_t  testState = IDLE;  // Current device state


#include "leds.h"
#include "tests.h"
#include "button.h"
#include "display.h"

Leds    leds;
Button  buttonUp;
Button  buttonDown;
Button  buttonTest;


void setup() {
  #ifdef DEBUG
    Serial.begin(BAUDRATE);
    Serial.println("[Main] Setup start");
  #endif

  leds.initializeLeds();
  leds.resetResult();

  buttonUp.initialize(BUTTON_UP, 100);
  buttonDown.initialize(BUTTON_DOWN, 100);
  buttonTest.initialize(BUTTON_TEST, 100);

  lcd.init();
  lcd.backlight();
  
  lcd.clear();
  displyText(0, 4, "Cable Tester");
  displyText(1, 8, "v1.3");
  delay(2000);
  lcd.clear();
  resetMenu();
  
  initializeCablePins();
  
  pageCount = ceil((float)totalTests/4);
  
  #ifdef DEBUG
    Serial.println("[Main] Setup complete");
  #endif
}

void loop() {
  buttonTest.read();
  if(buttonTest.wasReleased()) {
    if(buttonTest.pressedFor(2000)) { // Reset test
      #ifdef DEBUG
        Serial.println("[TS] IDLE");
      #endif
      testState = IDLE;
      resetMenu();
    }
    else {
      if(testState == IDLE || testState == COMPLETE) {
        lcd.clear();
        displyText(0, 6, "Testing");
        leds.resetResult();
        uint8_t index = getSelectedIndex();
        
        getTestInfo(singleTest, index);
        deserializeJson(doc, singleTest);
        delay(100);
        
        const char *relation = doc["R"];
        lastIndex = doc["p"].as<uint8_t>();

        if(strcmp(relation, "L") == 0)
          testType = 0;
        else if(strcmp(relation, "LS") == 0)
          testType = 1;
        else if(strcmp(relation, "NL") == 0)
          testType = 2;
        else if(strcmp(relation, "M") == 0) {
          testType = 3;
        }
          
        if(testType > 0) {
          JsonArray skipListArr =  doc["S"];
          skipCount = skipListArr.size();
          for(uint8_t i=0; i<skipCount; i++) {
            skipList[i] = skipListArr[i];
          }
        }
        if(testType == 3) {
          JsonArray multiListArr;
          JsonArray outArr =  doc["O"];
          outCount = outArr.size();
          for(uint8_t i=0; i<outCount; i++) {
            multiListArr =  outArr[i];
            pCount[i] = multiListArr.size();
            for(uint8_t x=0; x<pCount[i]; x++) {
              multipleList[i][x] = multiListArr[x];
            }
          }
        }
        
        errorCount = 0;
        currentPinIndex = 255;
        disableAllOutputs();
        preResult = curResult = NONE; // reset test results
        testState = TESTING;
      }
      
      #ifdef DEBUG
        Serial.println("[TS] TESTING Start");
      #endif
    }
  }

  if(testState == TESTING) {
    if(testUpdate()) {
      testState = COMPLETE;
      if(errorCount > 0) {
        curResult = POSITIVE;
      }
      else {
        curResult = NEGATIVE;
      }
      errorDisplayedCount = 0;
      lastErrorDisplay = millis();
      #ifdef DEBUG
        Serial.println("[TS] COMPLETE");
      #endif
      lcd.clear();
      displyText(0, 3, "Test Complete");
      lcd.setCursor(0, 1);
      lcd.printstr(doc["N"]);
      leds.showResult();
      // // uncomment following to display error msgs of serial monitor 
      // for(uint8_t i=0; i<errorCount; i++) {
      //   Serial.println(errorsList[i]);
      //   delay(10);
      // }
      delay(2000);
    }
  }

  else if(testState == COMPLETE) {
    if(millis() - lastErrorDisplay >= errorDisplayDelay) {
      clearLcd(2, 0, 3, 19);
      lcd.setCursor(0, 2);
      // Serial.println(errorDisplayedCount);
      lcd.printstr(errorsList[errorDisplayedCount]);
      errorDisplayedCount ++;
      lastErrorDisplay = millis();
    }
    
    if(errorDisplayedCount == errorCount) {
      testState = TESTING;
      errorCount = 0;
      currentPinIndex = 255;
      disableAllOutputs();
      // testState = IDLE;
      // resetMenu();
      delay(1000);
      #ifdef DEBUG
        Serial.println("[TS] IDLE");
      #endif
    }
  }
  else if(testState == IDLE) {
    buttonUp.read();
    buttonDown.read();

    if(buttonUp.wasReleased() || buttonUp.isLongPress(300)) {
      #ifdef DEBUG
        Serial.println("[Button] Up");
      #endif
      moveCursor(true);
    }
    else if(buttonDown.wasReleased() || buttonDown.isLongPress(300)) {
      #ifdef DEBUG
        Serial.println("[Button] Down");
      #endif
      moveCursor();
    }
  }
}
