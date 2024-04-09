#ifndef RollerGroupSimple_h
#define RollerGroupSimple_h

#include "shared.h"



/*
Metody do sterowania logika
toggle, upOrStop, downOrStop
*/
class RollerGroupLogic {
public:
  RollerGroupLogic(callbackFunction _rollersUp = NULL, callbackFunction _rollersDown = NULL, callbackFunction _rollersStop = NULL) {
    this->rollersUpCallback = _rollersUp;
    this->rollersDownCallback = _rollersDown;
    this->rollersStopCallback = _rollersStop;
  }

  /*Akcja*/
  // Metoda ktora podnosi rolete albo zatrzymuje
  static void upOrStop(void *__this) {
    RollerGroupLogic *_this = (RollerGroupLogic *)__this;
    
    if (timeElapsed - _this->stateStartedAt > ROLLER_GROUP_STOP_ALLOWED) {
      _this->rollersUpCallback();
      _this->stateStartedAt = timeElapsed;
    } else {
      _this->rollersStopCallback();
      _this->stateStartedAt = 0;
    }
  }

  /*Akcja*/
  // Metoda ktora podnosi rolete albo zatrzymuje
  static void downOrStop(void *__this) {
    RollerGroupLogic *_this = (RollerGroupLogic *)__this;

    if (timeElapsed - _this->stateStartedAt > ROLLER_GROUP_STOP_ALLOWED) {
      _this->rollersDownCallback();
      _this->stateStartedAt = timeElapsed;
    }else{
      _this->rollersStopCallback();
      _this->stateStartedAt = 0;
    }
  }

protected:
  callbackFunction rollersUpCallback = NULL;
  callbackFunction rollersDownCallback = NULL;
  callbackFunction rollersStopCallback = NULL; 
  unsigned long stateStartedAt = 0;
};

#endif
