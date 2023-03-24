void initializeCablePins() {
  for (uint8_t i = 0; i < totalPins; i++) {
    pinMode(inputPins[i], INPUT);    // connect external pull down resistor
    pinMode(outputPins[i], OUTPUT);  // pull down
    expectedState[i] = false;
  }
}

void disableAllOutputs() {
  for (uint8_t i = 0; i < totalPins; i++) {
    digitalWrite(outputPins[i], LOW);
    expectedState[i] = false;
  }
}

bool checkInputs() {
  bool errorFound = false;
  for (uint8_t i = 0; i < totalPins; i++) {
    if (digitalRead(inputPins[i]) != expectedState[i]) {
      strcat(errorsList[errorCount], (String(i + 1) + ",").c_str());
      errorFound = true;
      delay(10);
    }
    // Serial.print(" " + String(i+1));
  }
  // Serial.println();
  return errorFound;
}


bool isAvailable(uint8_t val) {
  for (uint8_t i = 0; i < skipCount; i++) {
    if (skipList[i] == val)
      return true;
  }
  return false;
}

bool testUpdate() {
  currentPinIndex++;
  if(currentPinIndex < 26) {
    // Serial.print("OUTPUT: " + String(currentPinIndex + 1) + " -> INPUTs:");
    strcpy(errorsList[errorCount], ("In:" + String(currentPinIndex + 1) + " Out: ").c_str());
    if (currentPinIndex < lastIndex) {
      uint8_t resetIndex = currentPinIndex;
      switch (testType) {
        case 0:
          digitalWrite(outputPins[currentPinIndex], HIGH);
          expectedState[currentPinIndex] = true;
          break;
        case 1:
          if (!isAvailable(currentPinIndex+1)) {
            digitalWrite(outputPins[currentPinIndex], HIGH);
            expectedState[currentPinIndex] = true;
          }
          break;
        case 2:
          if(skipList[currentPinIndex] != 0) {
            digitalWrite(outputPins[currentPinIndex], HIGH);
            resetIndex = skipList[currentPinIndex]-1;
            expectedState[resetIndex] = true;
          }
          break;
        case 3:
          if (!isAvailable(currentPinIndex+1)) {
            digitalWrite(outputPins[currentPinIndex], HIGH);
            for(uint8_t i=0; i<pCount[currentPinIndex]; i++) {
              expectedState[multipleList[currentPinIndex][i] - 1] = true;
            }
          }
          break;
      }

      if (checkInputs()) {
        errorCount++;
      }
      digitalWrite(outputPins[currentPinIndex], LOW);
      expectedState[resetIndex] = false;
      
      if(testType == 3) {
        for(uint8_t i=0; i<pCount[currentPinIndex]; i++) {
          expectedState[multipleList[currentPinIndex][i] - 1] = false;
        }
      }
    }
    else {
      digitalWrite(outputPins[currentPinIndex], HIGH);
      if (checkInputs()) {
        errorCount++;
      }
      digitalWrite(outputPins[currentPinIndex], LOW);
    }
    return false;
  }
  return true;
}
