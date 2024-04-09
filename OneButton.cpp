/**
 * @file OneButton.cpp
 *
 * @brief Library for detecting button clicks, doubleclicks and long press
 * pattern on a single button.
 *
 * @author Matthias Hertel, https://www.mathertel.de
 * @Copyright Copyright (c) by Matthias Hertel, https://www.mathertel.de.
 *                          Ihor Nehrutsa, Ihor.Nehrutsa@gmail.com
 *
 * This work is licensed under a BSD style license. See
 * http://www.mathertel.de/License.aspx
 *
 * More information on: https://www.mathertel.de/Arduino/OneButtonLibrary.aspx
 *
 * Changelog: see OneButton.h
 */

#include "OneButton.h"

// save function for click event
void OneButton::attachClick(callbackFunction newFunction)
{
  _clickFunc = newFunction;
} // attachClick


// save function for parameterized click event
void OneButton::attachClick(parameterizedCallbackFunction newFunction, void *parameter)
{
  _paramClickFunc = newFunction;
  _clickFuncParam = parameter;
} // attachClick


// save function for doubleClick event
//void OneButton::attachDoubleClick(callbackFunction newFunction)
//{
//  _doubleClickFunc = newFunction;
//  //_maxClicks = max(_maxClicks, 2);
//} // attachDoubleClick
//
//
//// save function for parameterized doubleClick event
//void OneButton::attachDoubleClick(parameterizedCallbackFunction newFunction, void *parameter)
//{
//  _paramDoubleClickFunc = newFunction;
//  _doubleClickFuncParam = parameter;
//  //_maxClicks = max(_maxClicks, 2);
//} // attachDoubleClick


// save function for multiClick event
// void OneButton::attachMultiClick(callbackFunction newFunction)
// {
//   _multiClickFunc = newFunction;
//   //_maxClicks = max(_maxClicks, 100);
// } // attachMultiClick


// save function for parameterized MultiClick event
// void OneButton::attachMultiClick(parameterizedCallbackFunction newFunction, void *parameter)
// {
//   _paramMultiClickFunc = newFunction;
//   _multiClickFuncParam = parameter;
//   //_maxClicks = max(_maxClicks, 100);
// } // attachMultiClick


// save function for longPressStart event
void OneButton::attachLongPressStart(callbackFunction newFunction)
{
  _longPressStartFunc = newFunction;
} // attachLongPressStart


// save function for parameterized longPressStart event
void OneButton::attachLongPressStart(parameterizedCallbackFunction newFunction, void *parameter)
{
  _paramLongPressStartFunc = newFunction;
  _longPressStartFuncParam = parameter;
} // attachLongPressStart


// // save function for longPressStop event
// void OneButton::attachLongPressStop(callbackFunction newFunction)
// {
//   _longPressStopFunc = newFunction;
// } // attachLongPressStop


// // save function for parameterized longPressStop event
// void OneButton::attachLongPressStop(parameterizedCallbackFunction newFunction, void *parameter)
// {
//   _paramLongPressStopFunc = newFunction;
//   _longPressStopFuncParam = parameter;
// } // attachLongPressStop


// // save function for during longPress event
// void OneButton::attachDuringLongPress(callbackFunction newFunction)
// {
//   _duringLongPressFunc = newFunction;
// } // attachDuringLongPress


// // save function for parameterized during longPress event
// void OneButton::attachDuringLongPress(parameterizedCallbackFunction newFunction, void *parameter)
// {
//   _paramDuringLongPressFunc = newFunction;
//   _duringLongPressFuncParam = parameter;
// } // attachDuringLongPress


void OneButton::reset(void)
{
  _state = OneButton::OCS_INIT;
  _nClicks = 0;
  _startTime = 0;
}


// ShaggyDog ---- return number of clicks in any case: single or multiple clicks
int OneButton::getNumberClicks(void)
{
  return _nClicks;
}


/**
 * @brief Debounce input pin level for use in SpesialInput.
  @todo check this bool value, if not working declare it as int
 */
uint8_t OneButton::debounce(uint8_t value, unsigned long *timeElapsed) {
    //if clicks are registered wrongly uncomment line bellow and check it again
//    *timeElapsed = millis(); // current (relative) time in msecs.
    if (_lastDebouncePinLevel == value) {
      if (*timeElapsed - _lastDebounceTime >= ONEBUTTON_DEBOUNCE_MS)
        debouncedPinLevel = value;
    } else {
      _lastDebounceTime = *timeElapsed;
      _lastDebouncePinLevel = value;
    }
    return debouncedPinLevel;
};

void OneButton::tick(bool activeLevel, unsigned long *timeElapsed)
{
  _fsm(debounce(activeLevel, timeElapsed), timeElapsed);
}


/**
 *  @brief Advance to a new state and save the last one to come back in cas of bouncing detection.
 */
void OneButton::_newState(stateMachine_t nextState)
{
  _state = nextState;
} // _newState()


/**
 * @brief Run the finite state machine (FSM) using the given level.
 */
void OneButton::_fsm(bool activeLevel, unsigned long *timeElapsed)
{
  unsigned long waitTime = (*timeElapsed - _startTime);

  // Implementation of the state machine
  switch (_state) {
  case OneButton::OCS_INIT:
    // waiting for level to become active.
    if (activeLevel) {
      _newState(OneButton::OCS_DOWN);
      _startTime = *timeElapsed; // remember starting time
      _nClicks = 0;
    } // if
    break;

  case OneButton::OCS_DOWN:
    // waiting for level to become inactive.

    if (!activeLevel) {
      _newState(OneButton::OCS_UP);
      _startTime = *timeElapsed; // remember starting time

    } else if ((activeLevel) && (waitTime > ONEBUTTON_PRESS_MS)) {
      if (_longPressStartFunc) _longPressStartFunc();
      if (_paramLongPressStartFunc) _paramLongPressStartFunc(_longPressStartFuncParam);
      _newState(OneButton::OCS_PRESS);
    } // if
    break;

  case OneButton::OCS_UP:
    // level is inactive

    // count as a short button down
    _nClicks++;
    _newState(OneButton::OCS_COUNT);
    break;

  case OneButton::OCS_COUNT:
    // dobounce time is over, count clicks

    if (activeLevel) {
      // button is down again
      _newState(OneButton::OCS_DOWN);
      _startTime = *timeElapsed; // remember starting time

    } else if ((waitTime >= ONEBUTTON_CLICK_MS) || (_nClicks == ONEBUTTON_MAXCLICKS)) {
      // now we know how many clicks have been made.

      if (_nClicks == 1) {
        // this was 1 click only.
        if (_clickFunc) _clickFunc();
        if (_paramClickFunc) _paramClickFunc(_clickFuncParam);

//      } else if (_nClicks == 2) {
//        // this was a 2 click sequence.
//        if (_doubleClickFunc) _doubleClickFunc();
//        if (_paramDoubleClickFunc) _paramDoubleClickFunc(_doubleClickFuncParam);

      } else {
        // // this was a multi click sequence.
        // if (_multiClickFunc) _multiClickFunc();
        // if (_paramMultiClickFunc) _paramMultiClickFunc(_multiClickFuncParam);
      } // if

      reset();
    } // if
    break;

  case OneButton::OCS_PRESS:
    // waiting for pin being release after long press.

    if (!activeLevel) {
      _newState(OneButton::OCS_PRESSEND);
      _startTime = *timeElapsed;

    } else {
      // still the button is pressed
      //if (_duringLongPressFunc) _duringLongPressFunc();
      //if (_paramDuringLongPressFunc) _paramDuringLongPressFunc(_duringLongPressFuncParam);
    } // if
    break;

  case OneButton::OCS_PRESSEND:
    // button was released.

    //if (_longPressStopFunc) _longPressStopFunc();
    //if (_paramLongPressStopFunc) _paramLongPressStopFunc(_longPressStopFuncParam);
    reset();
    break;

  default:
    // unknown state detected -> reset state machine
    _newState(OneButton::OCS_INIT);
    break;
  } // if

} // OneButton.tick()


// end.
