#ifndef PCFRelayRecord_h
#define PCFRelayRecord_h
#include "shared.h"
#include "PCF8575Home.h"


class PCFRelayRecord {
public:

  PCFRelayRecord(uint8_t _ioAddress /*, char *_name, mysensors_sensor_t _type*/) {
    //this->name = _name;
    //this->type = _type;

    this->state = false;
    this->ioAddress = _ioAddress;  //remove this variable to spare some bytes

    PCFRelayRecordList.push_back(this);
  }

  /*
  Uruchamiane w setup()
*/
  void configure() {
    HomeAddressIO _address(this->ioAddress);

    // for (PCF8575Home *pcf : PCF8575HomeList) {
    //   if (pcf->getAddress() == _address.getPcfAddress() && pcf->getLine() == _address.getWireLine()) {
    //     this->pcfInstance = pcf;
    //     break;
    //   }
    // }

    homeSystem.asOutput(this->ioAddress);
    // this->pcfInstance->pinMode(_address.getPcfPort(), OUTPUT); TODO: remove this
    // this->pcfInstance->digitalWrite(_address.getPcfPort(), HIGH);TODO: remove this
  }

  void setOn() {
    homeSystem.turnOn(this->ioAddress);

    this->state = true;
  }

  void setOff() {
    homeSystem.turnOff(this->ioAddress);
    //this->pcfInstance->digitalWrite(HomeAddressIO::decodePortAddress(this->ioAddress), HIGH);
    this->state = false;
  }

  bool isOn() {
    return this->state;
  }

  // void attachPcfInstance(PCF8575Home *_pcfInstance) {
  //   this->pcfInstance = _pcfInstance;
  // }

  char getIOAddress() {
    return this->ioAddress;
  }


protected:
  //PCF8575Home *pcfInstance = NULL;

  uint8_t ioAddress;
//const char *name;
#ifdef MY_SENSORS_ON
  mysensors_sensor_t type;
#endif
  bool state;
};

#endif