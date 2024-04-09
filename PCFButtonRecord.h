#ifndef PCFButtonRecord_h
#define PCFButtonRecord_h

#include "shared.h"

#ifdef MY_SCANPCF
class PCFButtonRecord {
public:

  PCFButtonRecord(char _ioAddress) {
    this->ioAddress = _ioAddress;

    PCFButtonRecordList.push_back(this);
  }

  void tick(bool _pressed, unsigned long *timeElapsed) {

    if (this->pressed != _pressed) {
      Serial.print("Wykryto zmiane na adresie ");
      Serial.println(this->ioAddress);
    }
    this->pressed = _pressed;
  }

  void update(unsigned long *timeElapsed) {}

  uint8_t getIOAddress() {
    return this->ioAddress;
  }

  bool getState() {
    return pressed;
  }
protected:
  uint8_t ioAddress;
  bool pressed = true;  //activeLow=true as OneButton does in sourcecode
};

#else
class PCFButtonRecord {
public:

  PCFButtonRecord(/*char *_name,*/ char _ioAddress, void *clickFnContext = NULL, parameterizedCallbackFunction function = NULL) {
    //this->name = _name;

    //it's a bad solution, but I made it for the sake of easy configuration;
    this->ioAddress = _ioAddress;
    this->pressed = false;

    if(function != NULL && clickFnContext != NULL){
      this->attachClick(clickFnContext, function);
    }
  
    PCFButtonRecordList.push_back(this);
  }

  PCFButtonRecord *attachClick(void *clickFnContext, parameterizedCallbackFunction function) {
    this->button.attachClick(function, clickFnContext);
    return this;
  }

//  PCFButtonRecord *attachDoubleClick(void *clickFnContext, parameterizedCallbackFunction function) {
//    this->button.attachDoubleClick(function, clickFnContext);
//    return this;
//  }

  // PCFButtonRecord* attachMultiClick(void *clickFnContext, parameterizedCallbackFunction function){
  //   this->button.attachMultiClick(function, clickFnContext);
  //   return this;
  // }

  PCFButtonRecord *attachLongPressStart(void *clickFnContext, parameterizedCallbackFunction function) {
    this->button.attachLongPressStart(function, clickFnContext);
    return this;
  }
  // PCFButtonRecord* attachDuringLongPress(void *clickFnContext, parameterizedCallbackFunction function){
  //   this->button.attachDuringLongPress(function, clickFnContext);
  //   return this;
  // }
  // PCFButtonRecord* attachLongPress(void *clickFnContext, parameterizedCallbackFunction function){
  //   this->button.attachLongPressStop(function, clickFnContext);
  //   return this;
  // }

  void tick(bool _pressed, unsigned long *timeElapsed) {
    //if(this->pressed != _pressed){
      this->button.tick(_pressed, timeElapsed);
      this->pressed = _pressed;
    //}
  }

  void update(unsigned long *timeElapsed) {
    this->button.tick(this->pressed, timeElapsed);
    //Serial.print(this->pressed);
  }

  uint8_t getIOAddress() {
    return this->ioAddress;
  }

  bool getState() {
    return pressed;
  }
protected:
  //char *name;
  uint8_t ioAddress;

  bool pressed = false;  //activeLow=true as OneButton does in sourcecode
  OneButton button;
};
#endif

#endif
