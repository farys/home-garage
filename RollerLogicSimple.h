#ifndef RollerLogicSimple_h
#define RollerLogicSimple_h

#include "ButtonLogic.h"
#include "PCFRelayRecord.h"
#include "shared.h"

/*
Available actions:
up, down, toggle, upOrStop, downOrStop
*/
class RollerLogicSimple : private ButtonLogic {
public:
  RollerLogicSimple(PCFRelayRecord *_actionUpRelay, PCFRelayRecord *_actionDownRelay) {

    actionUpRelay = _actionUpRelay;
    actionDownRelay = _actionDownRelay;

    //executeRelays(this);
    rollerLogicSimpleList.push_back(this);
  }

  byte getId() {
    return this->actionUpRelay->getIOAddress();
  }

  /*Akcja*/
  //Metoda ktora podnosi rolete
  static void up(void *__this) {
    RollerLogicSimple *_this = (RollerLogicSimple *)__this;
    _this->state = UP;

    executeRelays(_this);

    tasker.setTimeout(RollerLogicSimple::stop, ROLLER_TIMEOUT, (int)__this);
  }

  /*Akcja*/
  //Metoda ktora podnosi rolete albo zatrzymuje
  static void upOrStop(void *__this) {
    RollerLogicSimple *_this = (RollerLogicSimple *)__this;

    if (_this->state == STOP) {
      _this->prevState = STOP;
      _this->state = UP;
    } else if (_this->state == UP) {
      _this->prevState = UP;
      _this->state = STOP;
    } else if (_this->state == DOWN) {
      _this->prevState = DOWN;
      _this->state = STOP;
    }

    executeRelays(_this);

    if (_this->state != STOP) {
      tasker.setTimeout(RollerLogicSimple::stop, ROLLER_TIMEOUT, (int)__this);
    } else {
      tasker.cancel(RollerLogicSimple::stop, (int)__this);
    }
  }

  /*Akcja*/
  //Metoda ktora podnosi rolete albo zatrzymuje
  static void downOrStop(void *__this) {
    RollerLogicSimple *_this = (RollerLogicSimple *)__this;

    if (_this->state == STOP) {
      _this->prevState = STOP;
      _this->state = DOWN;
    } else if (_this->state == DOWN) {
      _this->prevState = DOWN;
      _this->state = STOP;
    } else if (_this->state == UP) {
      _this->prevState = UP;
      _this->state = STOP;
    }

    executeRelays(_this);

    if (_this->state != STOP) {
      tasker.setTimeout(RollerLogicSimple::stop, ROLLER_TIMEOUT, (int)__this);
    } else {
      tasker.cancel(RollerLogicSimple::stop, (int)__this);
    }
  }

  /*Akcja*/
  //Metoda ktora opuszcza rolete
  static void down(void *__this) {
    RollerLogicSimple *_this = (RollerLogicSimple *)__this;
    _this->state = DOWN;

    executeRelays(_this);

    tasker.setTimeout(RollerLogicSimple::stop, ROLLER_TIMEOUT, (int)__this);
  }

  /*Akcja*/
  //Metoda ktora podnosi/opuszcza rolete na takiej samej zasadzie jak brama wjazdowa uruchom/zatrzymaj/uruchom
  static void toggle(void *__this) {
    RollerLogicSimple *_this = (RollerLogicSimple *)__this;

    if (_this->state == STOP && _this->prevState == UP) {
      _this->prevState = STOP;
      _this->state = DOWN;
    } else if (_this->state == STOP && _this->prevState == DOWN) {
      _this->prevState = STOP;
      _this->state = UP;
    } else if ((_this->state == UP || _this->state == DOWN) && _this->prevState == STOP) {
      _this->prevState = _this->state;
      _this->state = STOP;
    }

    executeRelays(_this);
#ifdef MY_DEBUG
    Serial.println(F("Roller new state: "));
    printState(_this->state);
#endif

    if (_this->state != STOP) {
      tasker.setTimeout(RollerLogicSimple::stop, ROLLER_TIMEOUT, (int)__this);
    } else {
      tasker.cancel(RollerLogicSimple::stop, (int)__this);
    }
  }

  /*Akcja*/
  static void stop(void *__this) {
    RollerLogicSimple *_this = (RollerLogicSimple *)__this;
    if (_this->state == STOP) return;

    _this->prevState = _this->state;
    _this->state = STOP;

    executeRelays(_this);

    tasker.cancel(RollerLogicSimple::stop, (int)__this);
  }

  //   void myDelay(unsigned long ms) {              // ms: duration
  //    unsigned long start = millis();           // start: timestamp
  //    for (;;) {
  //        unsigned long now = millis();         // now: timestamp
  //        unsigned long elapsed = now - start;  // elapsed: duration
  //        if (elapsed >= ms)                    // comparing durations: OK
  //            return;
  //    }
  //  }

  static RollerLogicSimple *getById(byte id) {
    for (RollerLogicSimple *record : rollerLogicSimpleList) {
      if (record->getId() == id) {
        return record;
      }
    }

    return NULL;
  }
protected:
  enum RollerState : uint8_t { UP = 0,
                               DOWN = 1,
                               STOP = 2 };
  RollerState state = STOP;
  RollerState prevState = UP;
  RollerState executedState = STOP;
  PCFRelayRecord *actionUpRelay;
  PCFRelayRecord *actionDownRelay;

  unsigned long stateStartedAt = 0;

  static void executeRelays(RollerLogicSimple *_this) {
#ifdef MY_DEBUG
    Serial.print(F("Roller execute state: "));
    printState(_this->state);
    Serial.print("Was state: ");
    printState(_this->prevState);
#endif


    if (_this->executedState == _this->state) return;

    if (_this->state == UP) {
      _this->actionDownRelay->setOff();
      delay(300);
      _this->actionUpRelay->setOn();
    }

    if (_this->state == DOWN) {
      _this->actionUpRelay->setOff();
      delay(300);
      _this->actionDownRelay->setOn();
    }

    if (_this->state == STOP) {
      _this->actionUpRelay->setOff();
      _this->actionDownRelay->setOff();
    }

    _this->stateStartedAt = timeElapsed;  //millis();
    _this->executedState = _this->state;
  }

  static void printState(byte _state, bool ln = true) {
    if (_state == UP) {
      Serial.print("UP");
    } else if (_state == DOWN) {
      Serial.print("Down");
    } else if (_state == STOP) {
      Serial.print("Stop");
    }

    if (ln == true) {
      Serial.println("");
    }
  }
};

#endif
