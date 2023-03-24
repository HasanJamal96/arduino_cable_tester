#include <string.h>
void getTestInfo(char arr[], uint8_t index) {
  for(uint16_t i=0; i<512; i++) {
    char x = pgm_read_dword(&test[index][i]);
    arr[i] = x;
    if(x == '}'){
      arr[i+1] = '\0';
      i = 512;
    }
  }
}


void displyText(uint8_t row, uint8_t col, const char *text) {
  lcd.setCursor(col, row);
  lcd.printstr(text);
}

void updateMenuPage() {
  lcd.clear();
  for(uint8_t i=0; i<4; i++) {
    lcd.setCursor(1, i);
    strcpy(singleTest, " ");
    uint8_t x = (currentPage*4) + i;
    if(x < totalTests) {
      getTestInfo(singleTest, x);
      deserializeJson(doc, singleTest);
      lcd.printstr(doc["N"]);
    }
  }
}

void resetMenu() {
  updateMenuPage();
  lcd.setCursor(0, cursorIndex);
  lcd.printstr(">");
}

void clearLcd(uint8_t startRow, uint8_t startCol, uint8_t endRow, uint8_t endCol){
  for(uint8_t i=startRow; i<=endRow; i++){
    lcd.setCursor(startCol, i);
    for(uint8_t x=startCol; x<=endCol; x++){
      lcd.printstr(" ");
    }
  }
}

void moveCursor(bool up = false) {
  bool updatePage = false;
  clearLcd(cursorIndex,0,cursorIndex,0);
  if(!up) {
    cursorIndex ++;
    if(cursorIndex > 3) {
      currentPage ++;
      if(currentPage >= pageCount) {
        currentPage = 0;
      }
      cursorIndex = 0;
      updatePage = true;
    }
    if(currentPage == pageCount-1) {
      if((totalTests-1)%4 != 0) {
        if(cursorIndex > ((totalTests-1)%4) - 1) {
          cursorIndex = 0;
          currentPage = 0;
          updatePage = true;
        }
      }
    }
  }
  else {
    cursorIndex--;
    if(cursorIndex > 250) {
      cursorIndex = 3;
      currentPage--;
      if(currentPage > 250) {
        currentPage = pageCount-1;
        if((totalTests-1)%4 != 0) {
          cursorIndex = ((totalTests-1)%4) - 1;
        }
      }
      updatePage = true;
    }
  }
  if(updatePage) {
    updateMenuPage();
  }
  lcd.setCursor(0, cursorIndex);
  lcd.printstr(">");
}

uint8_t getSelectedIndex() {
  return ((currentPage*4) + cursorIndex);
}