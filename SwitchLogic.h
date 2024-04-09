#ifndef SwitchLogic_h
#define SwitchLogic_h

#include "ButtonLogic.h"
#include "PCFRelayRecord.h"
#include "shared.h"
/*
Metody do sterowania logika
turnOn, turnOff, toggle
*/
class SwitchLogic : private ButtonLogic {
  enum SwitchState : uint8_t { ON = 0,
                               OFF = 1 };

public:
  SwitchLogic(PCFRelayRecord *_relay, unsigned char _timeoutHours = 0) {

    this->relay = _relay;
    this->timeoutHours = _timeoutHours;
    //executeRelays(this);
    switchLogicList.push_back(this);
  }

  /*Akcja*/
  //Metoda wlacza przekaznik
  static void turnOn(void *__this) {
    SwitchLogic *_this = (SwitchLogic *)__this;
    _this->state = SwitchState::ON;

    executeRelays(_this);
  }

  /*Akcja*/
  //Metoda wylacza przekaznik
  static void turnOff(void *__this) {
    SwitchLogic *_this = (SwitchLogic *)__this;
    _this->state = SwitchState::OFF;

    executeRelays(_this);
  }

  /*Akcja*/
  //Metoda ktora przelacza wlacz/wylacz
  static void toggle(void *__this) {
    SwitchLogic *_this = (SwitchLogic *)__this;

    if (_this->state == SwitchState::ON) {
      _this->state = SwitchState::OFF;
    } else {
      _this->state = SwitchState::ON;
    }

    executeRelays(_this);

#ifdef MY_DEBUG
    Serial.println(F("Switch new state: "));
    Serial.println((uint8_t)_this->state);
#endif
  }

  byte getId() {
    return this->relay->getIOAddress();
  }

  static SwitchLogic *getById(byte id) {
    for (SwitchLogic *record : switchLogicList) {
      if (record->getId() == id) {
        return record;
      }
    }

    return NULL;
  }
protected:
  SwitchState state = SwitchState::OFF;
  SwitchState executedState = SwitchState::OFF;
  PCFRelayRecord *relay;

  uint8_t timeoutHours = 0;
  unsigned long stateStartedAt = 0;

  static void executeRelays(SwitchLogic *_this) {
    if (_this->executedState == _this->state) return;

    if (_this->state == SwitchState::ON) {
#ifdef MY_DEBUG
      Serial.println("executeRelays::setOn action");
#endif
      _this->relay->setOn();
    }

    if (_this->state == SwitchState::OFF) {
#ifdef MY_DEBUG
      Serial.println("executeRelays::setOff action");
#endif
      _this->relay->setOff();
    }

    _this->stateStartedAt = timeElapsed;  //millis();
    _this->executedState = _this->state;
  }
};

#endif
