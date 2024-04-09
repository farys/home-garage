#ifndef HomeSystemIO_h
#define HomeSystemIO_h

#include "shared.h"
#include "PCF8575Home.h"

class HomeSystemIO {
public:

  void registerPcf(uint8_t pcfIOAddress) {
    HomeAddressIO _address(pcfIOAddress);

#ifdef MY_DEBUG
    Serial.print(F("Registering PCF at IO Address "));
    Serial.print(pcfIOAddress);
    Serial.print(F(" with native address: line "));
    Serial.print(_address.getWireLine());
    Serial.print(F(" and address pcf "));
    Serial.println(_address.getPcfAddress());
#endif

    PCF8575Home *pcf = this->getPcf8575Home(_address.getWireLine(), _address.getPcfAddress());
    pcf->setLine(_address.getWireLine());

    if (this->activeWireLine != _address.getWireLine()) {
      this->setActiveWireLine(_address.getWireLine());
    }

    pcf->begin(_address.getPcfAddress());

    PCF8575HomeList.push_back(pcf);

#ifdef MY_DEBUG
    Serial.print(F("Registered PCF at IO Address "));
    Serial.println(pcfIOAddress);
#endif
  }

  void asInput(uint8_t ioAddress) {
    HomeAddressIO _address(ioAddress);

    if (this->activeWireLine != _address.getWireLine()) {
      this->setActiveWireLine(_address.getWireLine());
    }

    PCF8575Home *pcf = this->getPcf8575Home(_address.getWireLine(), _address.getPcfAddress());
    pcf->pinMode(_address.getPcfPort(), INPUT_PULLUP);
  }

  void asOutput(uint8_t ioAddress) {
    HomeAddressIO _address(ioAddress);

    if (this->activeWireLine != _address.getWireLine()) {
      this->setActiveWireLine(_address.getWireLine());
    }

    PCF8575Home *pcf = this->getPcf8575Home(_address.getWireLine(), _address.getPcfAddress());
    pcf->attachRelay(_address.getPcfPort());
    //pcf->digitalWrite(_address.getPcfPort(), HIGH);
  }

  void turnOn(uint8_t ioAddress) {
    HomeAddressIO _address(ioAddress);

    if (this->activeWireLine != _address.getWireLine()) {
      this->setActiveWireLine(_address.getWireLine());
    }

    PCF8575Home *pcf = this->getPcf8575Home(_address.getWireLine(), _address.getPcfAddress());
#ifdef MY_DEBUG
    Serial.print(F("Setting output at PCF:"));
    Serial.print(ioAddress);
    Serial.println(F(" to inactive."));
    _address.printDetails();
#endif
    pcf->digitalWrite(_address.getPcfPort(), LOW);
#ifdef MY_DEBUG
    Serial.println(F("Changes applied successfully."));
#endif
  }

  void turnOff(uint8_t ioAddress) {
    HomeAddressIO _address(ioAddress);

    if (this->activeWireLine != _address.getWireLine()) {
      this->setActiveWireLine(_address.getWireLine());
    }

#ifdef MY_DEBUG
    Serial.print(F("Setting output at PCF:"));
    Serial.print(ioAddress);
    Serial.println(F(" to inactive."));
    _address.printDetails();
#endif
    PCF8575Home *pcf = this->getPcf8575Home(_address.getWireLine(), _address.getPcfAddress());
    pcf->digitalWrite(_address.getPcfPort(), HIGH);
#ifdef MY_DEBUG
    Serial.println(F("Changes applied successfully."));
#endif
  }

  bool isOn(uint8_t ioAddress) {
    HomeAddressIO _address(ioAddress);

    //there isn't any communication between pcf and arduino
    // if (this->activeWireLine != _address.getWireLine()) {
    //   this->setActiveWireLine(_address.getWireLine());
    // }

    PCF8575Home *pcf = this->getPcf8575Home(_address.getWireLine(), _address.getPcfAddress());
    return pcf->isPressed(_address.getPcfPort());
  }

  PCF8575Home *getPcf8575Home(uint8_t ioAddress) {
    HomeAddressIO _address(ioAddress);
    uint8_t index = this->_getPcfIndex(_address.getWireLine(), _address.getPcfAddress());
    return &pcf[index];
  }

  PCF8575Home *getPcf8575Home(uint8_t wireIndex, uint8_t pcfAddress) {
    uint8_t index = this->_getPcfIndex(wireIndex, pcfAddress);
    return &pcf[index];
  }

  void checkInputsAtLine(uint8_t wireIndex) {

#ifdef MY_DEBUG
    Serial.print(F("Checking inputs states at line "));
    Serial.println(String(wireIndex));
#endif

    if (this->activeWireLine != wireIndex) {
      this->setActiveWireLine(wireIndex);
    }

    for (int i = (wireIndex * PCF_MAX_ON_LINE), n = (wireIndex * PCF_MAX_ON_LINE) + PCF_MAX_ON_LINE; i < n; i++) {
      //checking wire line also because users can set wrong io addresses
      // and that will lead to jump in into this if execution even if the pcf not exists.
      if (pcf[i].hasAnyInputs() && pcf[i].getLine() == wireIndex) {

#ifdef MY_DEBUG
        Serial.print(F("Checking PCF8575 at address "));
        Serial.println(pcf[i].getAddress());
#endif

        pcf[i].checkIfPressed();
      }
    }
  }

  void display_freeram() {
    extern int __heap_start, *__brkval;
    int v;
#ifdef MY_DEBUG
    Serial.print(F("Free RAM: "));
    Serial.print(this->freeRam());
    Serial.println(F("B"));
#endif
  }

  int freeRam() {
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
  }

  void setActiveWireLine(uint8_t i) {

#ifdef MY_DEBUG
    Serial.print(F("Setting active wire line to "));
    Serial.println(i);
#endif

    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
    delay(1);

    this->activeWireLine = i;
  }

protected:
  PCF8575Home pcf[NUM_I2C_WIRES * PCF_MAX_ON_LINE];  //four lines with maximum 4 devices
  uint8_t activeWireLine = 255;

protected:
  uint8_t _getPcfIndex(uint8_t wireIndex, uint8_t pcfAddress) {
    return (wireIndex * PCF_MAX_ON_LINE) + pcfAddress - 32;
  }

  uint8_t getActiveWireLine() {
    return this->activeWireLine;
  }
};

#endif
