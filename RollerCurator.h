#ifndef RollerCurator_h
#define RollerCurator_h

#include "shared.h"



/*
Metody do sterowania logika
toggle, upOrStop, downOrStop
*/
struct RollerCuratorChannel {
  byte io;
  unsigned long startedAt;
};

class RollerCurator {
public:
  RollerCurator(const byte channelsCount, const byte rollerPerChannel) {
    this->channelsCount = channelsCount;
    this->rollersPerChannel = rollersPerChannel;

    memset(this->channels, 0, this->channelsCount * this->rollersPerChannel * sizeof(RollerCuratorChannel));
    // for (byte i = 0; i < this->channelsCount; i++) {
    //   for (byte y = 0; y < this->rollersPerChannel; y++) {
    //     this->channels[i][y] = 0;
    //   }
    // }
  }


  bool isAllowed(byte channel, byte io) {
    if (channel > this->channelsCount) {
#if defined(MY_DEBUG)
      Serial.println("Zapytanie o zgode dla rolety z kanalu, który nie jest obslugiwany. Zwieksz ilosc kanalow w shared.h");
#endif
        return false;
    }
    for (byte i = 0; i < this->rollersPerChannel; i++) {
      if(timeElapsed - this->channels[channel][i].startedAt >= ROLLER_TIMEOUT){
        this->channels[channel][i].startedAt = timeElapsed;
        this->channels[channel][i].io = io;
        return true;
      }
    }
    return false;
  }

  bool rollerStopped(byte channel, byte io) {
    if (channel > this->channelsCount) {
#if defined(MY_DEBUG)
      Serial.println("Zgloszenie zatrzymania rolety z kanalu, który nie jest obslugiwany. Zwieksz ilosc kanalow w shared.h");
#endif
        return false;
    }
    for (byte i = 0; i < this->rollersPerChannel; i++) {
      if(this->channels[channel][i].io == io){
        this->channels[channel][i].startedAt = 0;
        return true;
      };
    }
    return true;
  }

protected:
  const byte channelsCount;
  const byte rollersPerChannel;
  RollerCuratorChannel channels[channelsCount][rollerPerChannel];
};

#endif
