#define DEBUG true
#define BAUDRATE 115200

/*
  Arduino Mega 2560 default I2C pin are 20,21 (SDA, SCL)
*/


////////////////////////////////////////////////////////////// LCD

const uint8_t address = 0x27;
const uint8_t rows    = 4;
const uint8_t cols    = 20;

uint8_t cursorIndex = 0;
uint8_t pageCount   = 0;
uint8_t currentPage = 0;

////////////////////////////////////////////////////////////// LEDs

const uint8_t LED_RED     = A0; // Error
const uint8_t LED_GREEN   = A1; // No Error
const uint8_t LED_AMBER   = A2; // Previously Error

////////////////////////////////////////////////////////////// Buttons

const uint8_t BUTTON_UP     = A3;
const uint8_t BUTTON_DOWN   = A4;
const uint8_t BUTTON_TEST   = A5;


////////////////////////////////////////////////////////////// I/O Pins

const uint8_t totalPins = 26;

const uint8_t inputPins[totalPins]  = {
                                        13, 12, 11, 10, 9,
                                        8,  7,  6,  5,  4,
                                        3,  2,  14, 15, 16,
                                        17, 18, 19, 22, 23,
                                        24, 25, 26, 27, 28,
                                        29
                                      };
const uint8_t outputPins[totalPins] = {
                                        30, 31, 32, 33, 34,
                                        35, 36, 37, 38, 39,
                                        40, 41, 42, 43, 44,
                                        45, 46, 47, 48, 49,
                                        50, 51, 52, 53, A15,
                                        A14
                                      };



////////////////////////////////////////////////////////////// Test

bool firstTest = false;

char singleTest[512];
uint8_t errorCount = 0; 
uint8_t errorDisplayedCount = 0; 
uint8_t testType = 0;
uint8_t currentPinIndex;
uint8_t lastIndex;
uint8_t skipList[totalPins];
uint8_t skipCount = 0;
uint8_t expect[totalPins];
uint8_t expectCount = 0;
char errorsList[totalPins][80];
bool expectedState[totalPins];

uint8_t outList[totalPins];
uint8_t outCount = 0;
uint8_t multipleList[totalPins][totalPins];
uint8_t pCount[totalPins];

uint32_t lastErrorDisplay = 0;
const uint16_t errorDisplayDelay = 1000;
uint32_t completeAt;
uint32_t checkError;

typedef enum : uint8_t {
  NONE,
  POSITIVE, // with error
  NEGATIVE, // no error
}results_t;


typedef enum : uint8_t {
  IDLE,
  TESTING,
  COMPLETE,
}test_state_t;


const uint8_t totalTests = 38;
int testIndex = 0;

const uint16_t maxTestStringLen = 200;

const char test[totalTests][maxTestStringLen] PROGMEM = {
  {"{\"N\":\"LK25M\",\"p\":\"25\",\"R\":\"0\"}"},
  {"{\"N\":\"LK25>8 MXLR/TRS\",\"p\":\"24\",\"R\":\"0\"}"},
  {"{\"N\":\"8 FXLR/TRS>LK25\",\"p\":\"24\",\"R\":\"0\"}"},
  {"{\"N\":\"SIGNAL 8 WAY\",\"p\":\"24\",\"R\":\"0\"}"},
  {"{\"N\":\"SIGNAL 4 WAY\",\"p\":\"12\",\"R\":\"0\"}"},
  {"{\"N\":\"SIGNAL 2 WAY\",\"p\":\"6\",\"R\":\"0\"}"},
  {"{\"N\":\"XLR/TRS\",\"p\":\"3\",\"R\":\"0\"}"},
  {"{\"N\":\"DB25>XLRM TASCAM\",\"p\":\"24\",\"R\":\"2\",\"S\":[25,24,12,11,10,23,22,21,9,8,7,20,19,18,6,5,4,17,16,15,3,2,1,14]}"},
  {"{\"N\":\"DB25>XLRF TASCAM\",\"p\":\"25\",\"R\":\"2\",\"S\":[23,22,21,17,16,15,11,10,9,5,4,3,0,24,20,19,18,14,13,12,8,7,6,2,1]}"},
  {"{\"N\":\"DB25>XLRM YAMAHA\",\"p\":\"24\",\"R\":\"2\",\"S\":[10,1,14,10,2,15,12,3,16,13,4,17,21,5,18,23,6,19,24,7,20,25,8,21]}"},
  {"{\"N\":\"TRS-2 x XLRM\",\"p\":\"6\",\"R\":\"2\",\"S\":[3,1,3,3,2,3]}"},
  {"{\"N\":\"ETHERCON>4 x XLRM\",\"p\":\"12\",\"R\":\"2\",\"S\":[9,1,2,12,9,3,6,9,5,4,9,7,8]}"},
  {"{\"N\":\"5 WAY LOOM BNC\",\"p\":\"14\",\"R\":\"1\",\"S\":[3,6,9,12]}"},
  {"{\"N\":\"2 WAY LOOM BNC/RCA\",\"p\":\"5\",\"R\":\"1\",\"S\":[3]}"},
  {"{\"N\":\"CABLE BNC-RCA-TS\",\"p\":\"2\",\"R\":\"0\"}"},
  {"{\"N\":\"ETHERCON 8 Pin\",\"p\":\"8\",\"R\":\"0\"}"},
  {"{\"N\":\"MIDI CABLE\",\"p\":\"5\",\"R\":\"0\"}"},
  {"{\"N\":\"MIDI->XLRM\",\"p\":\"3\",\"R\":\"2\",\"S\":[2,5,4]}"},
  {"{\"N\":\"MIDI->XLRF\",\"p\":\"5\",\"R\":\"2\",\"S\":[0,1,0,3,2]}"},
  {"{\"N\":\"8 WAY SPEAKER CABLE\",\"p\":\"12\",\"R\":\"1\",\"S\":[1,4,7,10]}"},
  {"{\"N\":\"4 WAY SPEAKER CABLE\",\"p\":\"6\",\"R\":\"1\",\"S\":[1,4]}"},
  {"{\"N\":\"1 x SPEAKON 2 WAY\",\"p\":\"3\",\"R\":\"1\",\"S\":[1]}"},
  {"{\"N\":\"DO-FILL\",\"p\":\"18\",\"R\":\"2\",\"S\":[0,2,3,0,5,6,0,0,0,0,0,0,0,8,9,0,11,12]}"},
  {"{\"N\":\"DO-SUB\",\"p\":\"21\",\"R\":\"2\",\"S\":[0,2,3,0,0,0,0,5,6,0,0,0,0,8,9,0,0,0,0,11,12]}"},
  {"{\"N\":\"DOM F-XLR M\",\"p\":\"24\",\"R\":\"0\"}"},
  {"{\"N\":\"XLRF-DOM M\",\"p\":\"18\",\"R\":\"0\"}"},
  {"{\"N\":\"8 WAY XLR LOOM\",\"p\":\"24\",\"R\":\"0\"}"},
  {"{\"N\":\"4 WAY XLR LOOM\",\"p\":\"12\",\"R\":\"0\"}"},
  {"{\"N\":\"1 XLR CABLE\",\"p\":\"3\",\"R\":\"0\"}"},
  {"{\"N\":\"LK25 CABLE\",\"p\":\"24\",\"R\":\"0\"}"},
  {"{\"N\":\"LK25-XLRM FAN OUT\",\"p\":\"24\",\"R\":\"0\"}"},
  {"{\"N\":\"LK25-XLRF FAN OUT\",\"p\":\"24\",\"R\":\"0\"}"},
  {"{\"N\":\"ETHERCON 7 Pin\",\"p\":\"7\",\"R\":\"0\"}"},
  {"{\"N\":\"ETHERCON>4 x XLRM\",\"p\":\"12\",\"R\":\"2\",\"S\":[9,1,2,9,3,6,9,5,4,9,7,8]}"},
  {"{\"N\":\"4 x XLRF>ETHERCON\",\"p\":\"9\",\"R\":\"3\",\"S\":[],\"O\":[[2],[3],[5],[9],[8],[6],[11],[12],[1,4,7,10]]}"},
  {"{\"N\":\"TRS-2 x XLR F\",\"p\":\"3\",\"R\":\"3\",\"S\":[],\"O\":[[2],[5],[1,3,4,6]]}"},
  {"{\"N\":\"DB25>XLRF YAMAHA\",\"p\":\"3\",\"R\":\"3\",\"S\":[9,11],\"O\":[[2],[5],[8],[11],[14],[17],[20],[23],[0],[1,4],[0],[7],[10],[3],[6],[9],[12],[15],[18],[21],[24],[17],[16],[19],[22]]}"}
};
