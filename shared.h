#ifndef shared_h
#define shared_h
#include "Bonezegei_List.h"
#include <DallasTemperature.h>

class PCFButtonRecord;
class PCFRelayRecord;
class PCF8575Home;
class SwitchLogic;
class RollerLogicSimple;

bool doneDebug = false;

//czy wlaczyc obsluge czujnikow - zakomentuj
#define DS18B20_ACTIVE 1
#define DS_ONEWIRE_PIN 6
#define DS_STRONGPULLUP_PIN 12
#define DS_TEMPERATURE_PRECISION 12
#define DS_TEMPERATURE_INTERVAL_IN_MS 900000  //15min * 60 * 1000

//czy wlaczyc nadrozgaleznik - zakomentuj
#define TCAADDR 112
//ile rozgałęźników na linii - nie tykac
#define PCF_MAX_ON_LINE 4  //2//4 //2bits = 4 combinations
//ile linii z rozgałęźnikami - nie tykac
#define NUM_I2C_WIRES 4  //04//4

//czas po jakim rolety zatrzymuja sie [ms]
#define ROLLER_TIMEOUT 35000
//po jakim czasie po wlaczeniu grupy rolet można je zatrzymać [ms]
#define ROLLER_GROUP_STOP_ALLOWED 35000
//ile jest linii zasilajacych rolety (kazda linia zabezpieczona bezpiecznikiem)
#define ROLLER_CURATOR_SUPPLY_CHANNELS
//ile rolet moze jednoczesnie pracowac na jednym kanale(bezpieczniku)
#define ROLLER_CURATOR_ROLLERS_PER_CHANNEL 2


//czy wlaczyc tryb raportowania do komputera - zakomentuj
#define MY_DEBUG 1
//TRYB SKANOWANIA - zakomentuj
#define MY_SCANPCF 1
// id w HomeAssistant tego sterownika
#define MY_NODE_ID 1

#define MY_GATEWAY_SERIAL

#ifdef MY_SENSORS_ON
#include <MySensors.h>
#endif
enum class SwitchState : uint8_t { ON = 0,
                                   OFF = 1 };

#define TASKER_MAX_TASKS 16
#include "Tasker.h"
Tasker tasker(true);  // creates prioritizing tasker


OneWire oneWire(DS_ONEWIRE_PIN);
DallasTemperature sensors(&oneWire, DS_STRONGPULLUP_PIN);

// Adresy czujników

//ile czujnikow
#define DS_SENSORS_NUM 11
bool DsTemperaturesToNotify = false;
bool DsRequestedTemperatures = false;
unsigned long DsRequestedAt = 0;
const int DsDelayInMs = 750 / (1 << (12 - DS_TEMPERATURE_PRECISION));
//numery seryjne czujnikow
const DeviceAddress dsSensorsAddresses[DS_SENSORS_NUM] = {
  { 0x28, 0x64, 0xD8, 0x1F, 0x00, 0x00, 0x00, 0xC3 },  //kotlownia
  { 0x28, 0x94, 0x96, 0x1D, 0x00, 0x00, 0x00, 0xFD },  //warsztat
  { 0x28, 0x74, 0xFD, 0x1B, 0x00, 0x00, 0x00, 0xBC },  //sypialnia
  { 0x28, 0xAC, 0x69, 0x1D, 0x00, 0x00, 0x00, 0x78 },  //warsztat pietro
  { 0x28, 0xEA, 0x11, 0x20, 0x00, 0x00, 0x00, 0x39 },  //wiatrolap
  { 0x28, 0x9A, 0x45, 0x1C, 0x00, 0x00, 0x00, 0x71 },  //p1
  { 0x28, 0xFA, 0x52, 0x1E, 0x00, 0x00, 0x00, 0x98 },  //korytarz pietro
  { 0x28, 0xCE, 0x30, 0x1C, 0x00, 0x00, 0x00, 0x3C },  //lazienka parter
  { 0x28, 0x33, 0xD5, 0x1D, 0x00, 0x00, 0x00, 0x83 },  //lazienka pietro
  { 0x28, 0x97, 0xA6, 0x1D, 0x00, 0x00, 0x00, 0x20 },  //korytarz parter
  { 0x28, 0xFF, 0xDF, 0x1D, 0x00, 0x00, 0x00, 0xDC }   //p2
};
float dsLastTemperatures[DS_SENSORS_NUM];
byte dsErrorsTemperatures[DS_SENSORS_NUM];

const int IRQWIREPIN[] = { 2, 3, 18, 19 };
volatile bool irqWireFlag[] = { false, false, false, false };

unsigned long timeElapsed;
/* List of Registered PCF buttons */
static Bonezegei_List<PCFButtonRecord*> PCFButtonRecordList;
/* List of Registered PCF relays */
static Bonezegei_List<PCFRelayRecord*> PCFRelayRecordList;
/* List of Registered PCF boards */
static Bonezegei_List<PCF8575Home*> PCF8575HomeList;
/* List of Registered logic switches */
static Bonezegei_List<SwitchLogic*> switchLogicList;
/* List of Registered logic switches */
static Bonezegei_List<RollerLogicSimple*> rollerLogicSimpleList;

#include "HomeSystemIO.h"
HomeSystemIO homeSystem;
#endif
