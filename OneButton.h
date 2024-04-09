// -----
// OneButton.h - Library for detecting button clicks, doubleclicks and long
// press pattern on a single button. This class is implemented for use with the
// Arduino environment. Copyright (c) by Matthias Hertel,
// http://www.mathertel.de This work is licensed under a BSD style license. See
// http://www.mathertel.de/License.aspx More information on:
// http://www.mathertel.de/Arduino
// -----
// 02.10.2010 created by Matthias Hertel
// 21.04.2011 transformed into a library
// 01.12.2011 include file changed to work with the Arduino 1.0 environment
// 23.03.2014 Enhanced long press functionalities by adding longPressStart and
// longPressStop callbacks
// 21.09.2015 A simple way for debounce detection added.
// 14.05.2017 Debouncing improvements.
// 25.06.2018 Optional third parameter for deactivating pullup.
// 26.09.2018 Anatoli Arkhipenko: Included solution to use library with other
// sources of input.
// 26.09.2018 Initialization moved into class declaration.
// 26.09.2018 Jay M Ericsson: compiler warnings removed.
// 29.01.2020 improvements from ShaggyDog18
// 07.05.2023 Debouncing in one point. #118
// -----

#ifndef OneButton_h
#define OneButton_h

#include <Arduino.h>

#define ONEBUTTON_DEBOUNCE_MS 30 // number of msecs for debounce times.50
#define ONEBUTTON_CLICK_MS 60   // number of msecs before a click is detected.400
#define ONEBUTTON_PRESS_MS 800   // number of msecs before a long button press is detected

#define ONEBUTTON_PRESSED 0 // this is the level of the input pin when the button is pressed.
                        // LOW if the button connects the input pin to GND when pressed.
                        // HIGH if the button connects the input pin to VCC when pressed.
#define ONEBUTTON_MAXCLICKS 2 // max number (1, 2, multi=3) of clicks of interest by registration of event functions.

// ----- Callback function types -----

extern "C" {
typedef void (*callbackFunction)(void);
typedef void (*parameterizedCallbackFunction)(void *);
}


class OneButton
{
public:
  
  // ----- Attach events functions -----

  /**
   * Attach an event to be called when a single click is detected.
   * @param newFunction This function will be called when the event has been detected.
   */
  void attachClick(callbackFunction newFunction);
  void attachClick(parameterizedCallbackFunction newFunction, void *parameter);

  /**
   * Attach an event to be called after a double click is detected.
   * @param newFunction This function will be called when the event has been detected.
   */
  //void attachDoubleClick(callbackFunction newFunction);
  //void attachDoubleClick(parameterizedCallbackFunction newFunction, void *parameter);

  /**
   * Attach an event to be called after a multi click is detected.
   * @param newFunction This function will be called when the event has been detected.
   */
  // void attachMultiClick(callbackFunction newFunction);
  // void attachMultiClick(parameterizedCallbackFunction newFunction, void *parameter);

  /**
   * Attach an event to fire when the button is pressed and held down.
   * @param newFunction
   */
  void attachLongPressStart(callbackFunction newFunction);
  void attachLongPressStart(parameterizedCallbackFunction newFunction, void *parameter);

  /**
   * Attach an event to fire as soon as the button is released after a long press.
   * @param newFunction
   */
  //void attachLongPressStop(callbackFunction newFunction);
  //void attachLongPressStop(parameterizedCallbackFunction newFunction, void *parameter);

  /**
   * Attach an event to fire periodically while the button is held down.
   * @param newFunction
   */
  //void attachDuringLongPress(callbackFunction newFunction);
  //void attachDuringLongPress(parameterizedCallbackFunction newFunction, void *parameter);

  // ----- State machine functions -----

  /**
   * @brief Call this function every time the input level has changed.
   * Using this function no digital input pin is checked because the current
   * level is given by the parameter.
   * Run the finite state machine (FSM) using the given level.
   */
  void tick(bool level, unsigned long *timeElapsed);


  /**
   * Reset the button state machine.
   */
  void reset(void);


  /*
   * return number of clicks in any case: single or multiple clicks
   */
  int getNumberClicks(void);


  /**
   * @return true if we are currently handling button press flow
   * (This allows power sensitive applications to know when it is safe to power down the main CPU)
   */
  bool isIdle() const { return _state == OCS_INIT; }

  /**
   * @return true when a long press is detected
   */
  bool isLongPressed() const { return _state == OCS_PRESS; };


private:


  // These variables will hold functions acting as event source.
  callbackFunction _clickFunc = NULL;
  parameterizedCallbackFunction _paramClickFunc = NULL;
  void *_clickFuncParam = NULL;

//  callbackFunction _doubleClickFunc = NULL;
//  parameterizedCallbackFunction _paramDoubleClickFunc = NULL;
//  void *_doubleClickFuncParam = NULL;

  // callbackFunction _multiClickFunc = NULL;
  // parameterizedCallbackFunction _paramMultiClickFunc = NULL;
  // void *_multiClickFuncParam = NULL;

  callbackFunction _longPressStartFunc = NULL;
  parameterizedCallbackFunction _paramLongPressStartFunc = NULL;
  void *_longPressStartFuncParam = NULL;

  // callbackFunction _longPressStopFunc = NULL;
  // parameterizedCallbackFunction _paramLongPressStopFunc = NULL;
  // void *_longPressStopFuncParam = NULL;

  // callbackFunction _duringLongPressFunc = NULL;
  // parameterizedCallbackFunction _paramDuringLongPressFunc = NULL;
  // void *_duringLongPressFuncParam = NULL;

  // These variables that hold information across the upcoming tick calls.
  // They are initialized once on program start and are updated every time the
  // tick function is called.

  // define FiniteStateMachine
  enum stateMachine_t : uint8_t {
    OCS_INIT = 0,
    OCS_DOWN = 1, // button is down
    OCS_UP = 2, // button is up
    OCS_COUNT = 3,
    OCS_PRESS = 6, // button is hold down
    OCS_PRESSEND = 7,
  };

  /**
   * Run the finite state machine (FSM) using the given level.
   */
  void _fsm(bool activeLevel, unsigned long *timeElapsed);

  /**
   *  Advance to a new state.
   */
  void _newState(stateMachine_t nextState);

  stateMachine_t _state = OCS_INIT;

  uint8_t debouncedPinLevel = 0;
  uint8_t _lastDebouncePinLevel = -1;      // used for pin debouncing
  uint8_t _lastDebounceTime = 0; // millis()

  unsigned long _startTime = 0; // start of current input change to checking debouncing
  uint8_t _nClicks = 0;             // count the number of clicks with this variable

public:
  stateMachine_t state() const { return _state; };
  uint8_t debounce(const uint8_t value, unsigned long *timeElapsed);
  uint8_t debouncedValue() const { return debouncedPinLevel; };
};

#endif
