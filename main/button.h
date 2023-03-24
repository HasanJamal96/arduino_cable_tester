#include <stdint.h>
#include "Arduino.h"
class Button {
  public:
    void initialize(uint8_t pin, uint32_t debounce) {
      _pin = pin;
      _debounce = debounce;
      pinMode(_pin, INPUT);
      _lastState = _state = digitalRead(_pin);
      _lastPressed = _lastChanged = millis();
      _changed = false;
    }

    bool read() {
      uint32_t ct = millis();
      bool pinVal = digitalRead(_pin);
      if(ct - _lastChanged < _debounce) {
        _changed = false;
      }
      else {
        _lastState = _state;
        _state = pinVal;
        _changed = (_state != _lastState);
        if(_changed) {
          _lastChanged = millis();
          if(_state) {
            _lastPressed = millis();
          }
        }
      }
      return _state;
    }
    
    bool isLongPress(uint32_t holdTime) {
      if(_state && (millis() - _lastPressed > holdTime)) {
        _lastPressed = millis();
        return true;
      }
      return false;
    }

    bool wasReleased() {
      return !_state && _changed;
    }

    bool pressedFor(uint32_t val) {
      return (millis() - _lastPressed >= val);
    }

  private:    
    uint8_t _pin;
    uint32_t _lastChanged;
    bool _state;
    bool _lastState;
    uint32_t _debounce;
    bool _changed;
    uint32_t _lastPressed;

};