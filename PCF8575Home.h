#ifndef PCF8575Home_h
#define PCF8575Home_h
#include "PCF8575.h"
#include "shared.h"
#include "PCFButtonRecord.h"

class PCF8575Home : public PCF8575 {
public:

  PCF8575Home()
    : PCF8575() {

    for (byte i = 0; i < 16; i++) {
      buttonInstances[i] = NULL;
    }
  }

  void attachButton(uint8_t _port, PCFButtonRecord *_buttonRecord) {
    this->buttonInstances[_port] = _buttonRecord;
    this->pinMode(_port, INPUT_PULLUP);  // in homesystem this is aswell
    this->_hasAnyInputs = true;
  }

  void attachRelay(uint8_t _port) {
    this->pinMode(_port, OUTPUT_PULLUP);
    outputPorts[_port] = true;
    /* Update GPIO values */
    updateGPIO();
  }

  void updateStates() {
    for (int i = 0; i < 16; i++) {
      if (outputPorts[i] == true) attachRelay(i);
      if (buttonInstances[i] != NULL) attachButton(i, buttonInstances[i]);
    }
  }

  uint8_t getAddress() {
    return this->_address;
  }

  void setLine(int8_t _line) {
    this->_i2cLine = _line;
  }

  uint8_t getLine() {
    return this->_i2cLine;
  }

  void checkIfPressed() {
    uint16_t buttonBits = this->read();
    bool _active;

    for (uint8_t i = 0; i < 16; i++) {
      if (this->buttonInstances[i] != NULL) {
        _active = ((~buttonBits) & ((uint16_t)(1 << (i)))) > 0;
        this->buttonInstances[i]->tick(_active, &timeElapsed);

#ifdef MY_DEBUG
        if (_active) {
          Serial.print(F("Got state pressed for IO "));
          uint8_t _ioAddress = HomeAddressIO::encodeAddress(this->getLine(), this->getAddress(), i);
          Serial.print(_ioAddress);
          Serial.print(" -> port ");
          Serial.println(i);
        }
#endif
      }
    }
  }

  bool isPressed(uint8_t index) {
    return this->buttonInstances[index]->getState();
  }

  bool hasAnyInputs() {
    return this->_hasAnyInputs;
  }

protected:
  PCFButtonRecord *buttonInstances[16];
  bool outputPorts[16];
  bool _hasAnyInputs = false;
  //is decoded from pcf io address by registerPcf method to a native value of wire line and pcfaddress
  uint8_t _i2cLine = 255;
};

#endif
