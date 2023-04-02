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
  displyText(1, 8, "v1.4");
  delay(500);
  lcd.clear();
  resetMenu();
  
  initializeCablePins();
  
  pageCount = ceil((float)totalTests/4);
  
  #ifdef DEBUG
    Serial.println("[Main] Setup complete");
  #endif
}

void idleState() {
  testState = IDLE;
  resetMenu();
  leds.resetResult();
  resetTestParams();
  clearAllErrors();
}

void clearAllErrors() {
  for(uint8_t i=0; i<totalPins; i++) {
    // errorsList[i][0] = ' ';
    strcpy(errorsList[i], " ");
  }
  Serial.println("cleared");
}

bool restart = false;
void loop() {
  buttonTest.read();
  if(buttonTest.wasReleased()) {
    if(testState == IDLE) {
      lcd.clear();
      displyText(0, 6, "Testing");
      leds.resetResult();
      uint8_t index = getSelectedIndex();
      
      getTestInfo(singleTest, index);
      deserializeJson(doc, singleTest);
      delay(100);
      
      testType = doc["R"].as<uint8_t>();
      lastIndex = doc["p"].as<uint8_t>();
      
      lcd.setCursor(0, 1);
      lcd.printstr(doc["N"]);
        
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
      
      resetTestParams();
      preResult = curResult = NONE; // reset test results
      testState = TESTING;
      clearAllErrors();
      firstTest = true;
    }
    
    else if(testState == TESTING) { // Reset errors if test button is pressed during testing
      clearLcd(2, 0, 3, 19);
      errorDisplayedCount = 0;
      resetTestParams();
      preResult = curResult = NONE;
      clearAllErrors();
      restart = false;
      firstTest = true;
    }
    #ifdef DEBUG
      Serial.println("[TS] TESTING Start");
    #endif
  }

  if(testState == TESTING) {
    if(!restart) {
      if(testUpdate()) {
        if(curResult == NEGATIVE) {
          clearLcd(2, 0, 3, 19);
          lcd.setCursor(0, 2);
          clearAllErrors();
        }
        leds.showResult();
        restart = true;
        completeAt = millis();
      }
    }
    if(millis() - checkError >= 20) {
      if(millis() - lastErrorDisplay >= errorDisplayDelay) {
          bool foundError = false;
          for(uint8_t i=errorDisplayedCount; i<totalPins; i++) {
            if(strcmp(errorsList[i], " ") != 0) {
              clearLcd(2, 0, 3, 19);
              lcd.setCursor(0, 2);
              lcd.printstr(errorsList[i]);
              errorDisplayedCount = i+1;
              foundError = true;
              lastErrorDisplay = millis();
              i = totalPins;
            }
          }
          if(errorDisplayedCount == totalPins) {
            errorDisplayedCount = 0;
          }
          if(!foundError) {
            errorDisplayedCount = 0;
          }
        }
      checkError = millis();
    }
  }
  if(restart) {
    if(millis() - completeAt >= 500) {
      restart = false;
    }
  }
  buttonUp.read();
  buttonDown.read();

  if(buttonUp.wasReleased() || buttonUp.isLongPress(300)) {
    #ifdef DEBUG
      Serial.println("[Button] Up");
    #endif
    if(testState != IDLE) {
      idleState();
    }
    else {
      moveCursor(true);
    }
  }
  else if(buttonDown.wasReleased() || buttonDown.isLongPress(300)) {
    #ifdef DEBUG
      Serial.println("[Button] Down");
    #endif
    if(testState != IDLE) {
      idleState();
    }
    else {
      moveCursor();
    }
  }
}
