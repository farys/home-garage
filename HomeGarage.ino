#include <Wire.h>  // Required for I2C communication

#include "HomeAddressIO.h"
#include "OneButton.h"
#include "RollerLogicSimple.h"
#include "RollerGroupLogic.h"
#include "SwitchLogic.h"
#include "PCFButtonRecord.h"
#include "PCFRelayRecord.h"
#include "PCF8575Home.h"

#ifdef MY_SCANPCF
#include "HomeScanner.h"
HomeScanner HS;
#endif

#include "shared.h"

#ifdef ARDUINO_AVR_MEGA2560
#endif
#ifdef ARDUINO_AVR_UNO
#endif

/* 
  PCF expanders configuration
  uncommend MY_SCANPCF(w pliku shared.h), send program, check the addresses of pcfs and write them here.
*/
const uint8_t _pcfIOAddresses[] = { /*0, 16, 32, 48, 80, 96, 112, 128, 192, 208, 224*/ };  //here pcfs addresses

//dom
//1domek1.

#ifndef ARDUINO_AVR_UNO

//KORYTARZ PARTER SWIATLO
PCFRelayRecord przekaznikSwiatloKorytarzParter(14);                   //OBIEKT PRZEKAZNIKA
SwitchLogic swiatloKorytarzParter(&przekaznikSwiatloKorytarzParter);  //OBIEKT - LOGIKA STERUJACA PRZEKAZNIKIEM (TU UZYTO SWITCHLOGIC)
PCFButtonRecord wlacznikKorytarzPrzyWarsztacieDol(35);                //WLACZNIK STERUJACY LOGIKA-WYWOLUJACY ROZKARY W OBIEKCIE LOGIKI
PCFButtonRecord wlacznikKorytarzPrzySchodachDol(94);                  //WLACZNIK STERUJACY LOGIKA-WYWOLUJACY ROZKARY W OBIEKCIE LOGIKI
PCFButtonRecord wlacznikKorytarzPrzyLazience(44);                     //WLACZNIK STERUJACY LOGIKA-WYWOLUJACY ROZKARY W OBIEKCIE LOGIKI
PCFButtonRecord wlacznikKorytarzPrzyWiatrolapie(35);

//LAZIENKA PARTER GLOWNE SWIATLO
PCFRelayRecord przekaznikLazienkaParterGlowne(6);  //, S_BINARY);
SwitchLogic swiatloLazParterGlowne(&przekaznikLazienkaParterGlowne);
PCFButtonRecord wlacznikLazienkaParterNaKorytarzu(42);

//LAMPA GARAZ PRZEJSCIE
PCFRelayRecord przekaznikGarazLampaPrzejscie(15);  //, S_BINARY);
SwitchLogic swiatloGarazPrzejscie(&przekaznikGarazLampaPrzejscie);
PCFButtonRecord wlacznikGarazPrzejscie(55);
PCFButtonRecord wlacznikLampaGarazWWiatrolapie(31);

//FIX PRAWY
PCFRelayRecord przekaznikRoletaFixPrawyGora(98);  //, S_BINARY);
PCFRelayRecord przekaznikRoletaFixPrawyDol(99);   //, S_BINARY);
RollerLogicSimple roletaFixPrawy(&przekaznikRoletaFixPrawyGora, &przekaznikRoletaFixPrawyDol);
PCFButtonRecord wlacznikFixPrawyRoletaGora(83);
PCFButtonRecord wlacznikFixPrawyRoletaDol(80);

PCFRelayRecord przekaznikRoletaFixLewyGora(113);  //, S_BINARY);
PCFRelayRecord przekaznikRoletaFixLewyDol(112);
RollerLogicSimple roletaFixLewy(&przekaznikRoletaFixLewyGora, &przekaznikRoletaFixLewyDol);
PCFButtonRecord wlacznikFixLewaRoletaGora(82);
PCFButtonRecord wlacznikFixLewaRoletaDol(81);

PCFRelayRecord przekaznikRoletaSalonPoludnieGora(96);  //, S_BINARY);
PCFRelayRecord przekaznikRoletaSalonPoludnieDol(97);   //, S_BINARY);
RollerLogicSimple roletaSalonPoludnie(&przekaznikRoletaSalonPoludnieGora, &przekaznikRoletaSalonPoludnieDol);
PCFButtonRecord wlacznikSalonOknoRoletaGora(84);
PCFButtonRecord wlacznikSalonOknoRoletaDol(86);

PCFRelayRecord przekaznikRoletaSalonHSPrawyGora(100);  //, S_BINARY);
PCFRelayRecord przekaznikRoletaSalonHSPrawyDol(101);   //, S_BINARY);
RollerLogicSimple roletaHSPrawy(&przekaznikRoletaSalonHSPrawyGora, &przekaznikRoletaSalonHSPrawyDol);
PCFButtonRecord wlacznikHSPrawaRoletaGora(93);
PCFButtonRecord wlacznikHSPrawaRoletaDol(90);

PCFRelayRecord przekaznikRoletaSalonHSLewyGora(106);  //, S_BINARY);
PCFRelayRecord przekaznikRoletaSalonHSLewyDol(107);
RollerLogicSimple roletaHSLewy(&przekaznikRoletaSalonHSLewyGora, &przekaznikRoletaSalonHSLewyDol);
PCFButtonRecord wlacznikHSLewaRoletaGora(92);
PCFButtonRecord wlacznikHSLewaRoletaDol(91);

PCFRelayRecord przekaznikRoletaGaraz2OknoGora(102);  //, S_BINARY);
PCFRelayRecord przekaznikRoletaGaraz2OknoDol(103);   //, S_BINARY);
RollerLogicSimple roletaGaraz2Okno(&przekaznikRoletaGaraz2OknoGora, &przekaznikRoletaGaraz2OknoDol);
PCFButtonRecord wlacznikGarazRoleta2Okno(57);

PCFRelayRecord przekaznikRoletaTarasGora(109);  //, S_BINARY);
PCFRelayRecord przekaznikRoletaTarasDol(108);   //, S_BINARY);
RollerLogicSimple roletaTaras(&przekaznikRoletaTarasGora, &przekaznikRoletaTarasDol);
PCFButtonRecord wlacznikTarasRoletaGora(85);
PCFButtonRecord wlacznikTarasRoletaDol(87);

PCFRelayRecord przekaznikRoletaWarsztatGora(110);  //, S_BINARY);
PCFRelayRecord przekaznikRoletaWarsztatDol(111);   //, S_BINARY);
RollerLogicSimple roletaWarsztat(&przekaznikRoletaWarsztatGora, &przekaznikRoletaWarsztatDol);
PCFButtonRecord wlacznikWarsztatOknoRoletaGora(33);
PCFButtonRecord wlacznikWarsztatOknoRoletaDol(32);

PCFRelayRecord przekaznikRoletaKuchniaOknoGora(115);  //, S_BINARY);
PCFRelayRecord przekaznikRoletaKuchniaOknoDol(114);   //, S_BINARY);
RollerLogicSimple roletaKuchniaOkno(&przekaznikRoletaKuchniaOknoGora, &przekaznikRoletaKuchniaOknoDol);
PCFButtonRecord wlacznikKuchniaOknoRoletaGora(46);  //
PCFButtonRecord wlacznikKuchniaOknoRoletaDol(95);   //






PCFButtonRecord wlacznikKuchniaDrzwiRoletaGora(45);
PCFButtonRecord wlacznikKuchniaDrzwiRoletaDol(88);

PCFButtonRecord wlacznikSalonLampaNadStolem1Bieg(26);
PCFButtonRecord wlacznikSalonLampaNadStolem2Bieg(27);

PCFButtonRecord wlacznikLampaSchodyDol(89);

PCFButtonRecord alarmKontraktronyPolnoc(128);

PCFButtonRecord wlacznikWiatrolapNaKorytarzu(34);

PCFButtonRecord wlacznikPustyWiatrolap(29);
PCFButtonRecord wlacznikBramaNaDworzeWWiatrolapie(28);

PCFButtonRecord wlacznikWiatrolapWWiatrolapie(30);

PCFButtonRecord wlacznikRoleta1Okno(56);
PCFButtonRecord wlacznikBramaWGarazu(59);
PCFButtonRecord wlacznikSwiatloNadGarazem(58);
PCFButtonRecord wlacznikSwiatloWkotlowni(60);
PCFButtonRecord wlacznikSwiatloWiatrolapWGarazu(62);
PCFButtonRecord wlacznikSwiatloGarazWGarazuPrzyDrzwiachWWiatrolapie(61);

//WLACZNIKI PIETRO
PCFButtonRecord wlacznikSypialniaRoletaGora(228);
PCFButtonRecord wlacznikSypialniaRoletaDol(229);
PCFButtonRecord wlacznikP2RoletaGora(227);
PCFButtonRecord wlacznikP2RoletaDol(225);
PCFButtonRecord wlacznikP1RoletaPoludnieGora(206);
PCFButtonRecord wlacznikP1RoletaPoludnieDol(205);
//PCFButtonRecord wlacznikP1RoletaZachodGora(228);
//PCFButtonRecord wlacznikP1RoletaZachodGora(228);
PCFButtonRecord wlacznikWarsztatPietorRoletaGora(230);
PCFButtonRecord wlacznikWarsztatPietroRoletaDol(231);
PCFButtonRecord wlacznikLazienkaRoletaGora(204);
PCFButtonRecord wlacznikLazienkaRoletaDol(203);
PCFButtonRecord wlacznikSwiatloKorytarzObokP2(226);
PCFButtonRecord wlacznikSwiatloKorytarzObokP1(207);
PCFButtonRecord wlacznikKorytarzRoletaGora(39);
PCFButtonRecord wlacznikKorytarzRoletaDol(38);
PCFButtonRecord wlacznikSwiatloKorytarzPietroPrzyOknie(47);
PCFButtonRecord wlacznikSwiatloSchody(37);

/*
  4) POWIAZANIA AKCJE DO PRZYCISKOW
*/
#endif
void actionBindings() {
#ifndef MY_SCANPCF

  //KORYTARZ PARTER SW
  wlacznikKorytarzPrzyWarsztacieDol.attachClick(&swiatloKorytarzParter, swiatloKorytarzParter.toggle);
  wlacznikKorytarzPrzySchodachDol.attachClick(&swiatloKorytarzParter, swiatloKorytarzParter.toggle);
  wlacznikKorytarzPrzyLazience.attachClick(&swiatloKorytarzParter, swiatloKorytarzParter.toggle);
  wlacznikKorytarzPrzyWiatrolapie.attachClick(&swiatloKorytarzParter, swiatloKorytarzParter.toggle);
  //LAZIENKA SW GLOWNE
  wlacznikLazienkaParterNaKorytarzu.attachClick(&swiatloLazParterGlowne, swiatloLazParterGlowne.toggle);

  //GARAZ PRZEJSCIE
  wlacznikGarazPrzejscie.attachClick(&swiatloGarazPrzejscie, swiatloGarazPrzejscie.toggle);
  wlacznikLampaGarazWWiatrolapie.attachClick(&swiatloGarazPrzejscie, swiatloGarazPrzejscie.toggle);

  //FIX LEWY
  wlacznikFixLewaRoletaGora.attachClick(&roletaFixLewy, roletaFixLewy.upOrStop);
  wlacznikFixLewaRoletaDol.attachClick(&roletaFixLewy, roletaFixLewy.downOrStop);

  //FIX PRAWY
  wlacznikFixPrawyRoletaGora.attachClick(&roletaFixPrawy, roletaFixPrawy.upOrStop);
  wlacznikFixPrawyRoletaDol.attachClick(&roletaFixPrawy, roletaFixPrawy.downOrStop);

  wlacznikWarsztatOknoRoletaGora.attachClick(&roletaWarsztat, roletaWarsztat.upOrStop);
  wlacznikWarsztatOknoRoletaDol.attachClick(&roletaWarsztat, roletaWarsztat.downOrStop);

  wlacznikSalonOknoRoletaGora.attachClick(&roletaSalonPoludnie, roletaSalonPoludnie.upOrStop);
  wlacznikSalonOknoRoletaDol.attachClick(&roletaSalonPoludnie, roletaSalonPoludnie.downOrStop);

  wlacznikHSPrawaRoletaGora.attachClick(&roletaHSPrawy, roletaHSPrawy.upOrStop);
  wlacznikHSPrawaRoletaDol.attachClick(&roletaHSPrawy, roletaHSPrawy.downOrStop);

  wlacznikHSLewaRoletaGora.attachClick(&roletaHSLewy, roletaHSLewy.upOrStop);
  wlacznikHSLewaRoletaDol.attachClick(&roletaHSLewy, roletaHSLewy.downOrStop);

  wlacznikGarazRoleta2Okno.attachClick(&roletaGaraz2Okno, roletaGaraz2Okno.toggle);

  wlacznikTarasRoletaGora.attachClick(&roletaTaras, roletaTaras.upOrStop);
  wlacznikTarasRoletaDol.attachClick(&roletaTaras, roletaTaras.downOrStop);

  wlacznikKuchniaOknoRoletaGora.attachClick(&roletaKuchniaOkno, roletaKuchniaOkno.upOrStop);   //up ddown toggle
  wlacznikKuchniaOknoRoletaDol.attachClick(&roletaKuchniaOkno, roletaKuchniaOkno.downOrStop);  //up ddown toggle

  wlacznikLampaGarazWWiatrolapie.attachClick(&swiatloGarazPrzejscie, swiatloGarazPrzejscie.toggle);

//  wlacznikRoleta.attachDoubleClick(ButtonLogic::print, "kliknieto podwojnie");
//  wlacznikRoleta.attachMultiClick(ButtonLogic::print, "kliknieto kilkukrotnie");
//  wlacznikRoleta.attachLongPressStart(ButtonLogic::print, "rozpoczeto przytrzymanie");
//  wlacznikRoleta.attachLongPress(ButtonLogic::print, "przytrzymano");
//  wlacznikRoleta.attachDuringLongPress(ButtonLogic::print, "klawisz trzymany");
#endif
}


////////////////////////////////////
//
// INTERRUPT ROUTINE + FLAG - interruptions when button is clicked - 4 interrupt lines
// pin number - name of pcf's group
// 2 - rozdzielnia garaz - poziom nizszy
// 3 - rozdzielnia garaz - poziom wyzszy
// 18 - rozdzielnia kotlownia
// 19 - rozdzielnia pietro
////////////////////////////////////

void pcfIrqWire() {
  irqWireFlag[0] = true;
}
void pcfIrqWire1() {
  irqWireFlag[1] = true;
}
void pcfIrqWire2() {
  irqWireFlag[2] = true;
}
void pcfIrqWire3() {
  irqWireFlag[3] = true;
}

void setup() {

#ifdef MY_DEBUG
  //available speeds: 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, and 921600
  Serial.begin(115200);
  Serial.println("Start of setup");
#endif

  Wire.begin();
  Wire.setWireTimeout(25000, false);
  Wire.setClock(100);

  //initializing PCFs objects
  uint8_t length = sizeof(_pcfIOAddresses) / sizeof(uint8_t);

  for (byte i = 0; i < length; i++) {
    homeSystem.registerPcf(_pcfIOAddresses[i]);
  }

  //configuring relays
  for (PCFRelayRecord *record : PCFRelayRecordList) {
    record->configure();
  }

  //configuring buttons
  HomeAddressIO _recordAddress;
  for (PCFButtonRecord *record : PCFButtonRecordList) {

    HomeAddressIO::decodeAddress(record->getIOAddress(), _recordAddress);
    PCF8575Home *pcf = homeSystem.getPcf8575Home(_recordAddress.getWireLine(), _recordAddress.getPcfAddress());
    homeSystem.asInput(record->getIOAddress());
    pcf->attachButton(_recordAddress.getPcfPort(), record);
  }

  timeElapsed = millis();

#ifdef MY_DEBUG
  Serial.println(F("started"));
#endif

  //buttons' actions bindings
  actionBindings();

  pinMode(IRQWIREPIN[0], INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IRQWIREPIN[0]), pcfIrqWire, CHANGE);
  pinMode(IRQWIREPIN[1], INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IRQWIREPIN[1]), pcfIrqWire1, CHANGE);
  pinMode(IRQWIREPIN[2], INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IRQWIREPIN[2]), pcfIrqWire2, CHANGE);
  pinMode(IRQWIREPIN[3], INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IRQWIREPIN[3]), pcfIrqWire3, CHANGE);


  for (uint8_t i = 0; i < NUM_I2C_WIRES; i++) {

#ifdef MY_DEBUG
    Serial.print(F("Loading states from wire "));
    Serial.println(i);
#endif

    homeSystem.checkInputsAtLine(i);
  }

#ifdef MY_DEBUG
  Serial.print(F("Configured buttons: "));
  for (PCFButtonRecord *button : PCFButtonRecordList) {
    Serial.print(button->getIOAddress());
    Serial.print(" ");
  }
  Serial.println("");
#endif

#ifdef DS18B20_ACTIVE

#ifdef MY_DEBUG
  Serial.print(F("Configured DS sensor pin: "));
  Serial.println(DS_ONEWIRE_PIN);
  Serial.print(F("Configured DS sensor precision: "));
  Serial.println(DS_TEMPERATURE_PRECISION);
  Serial.print(F("Configured DS sensors: "));
  for (byte i = 0; i < DS_SENSORS_NUM; i++) {
    Serial.print(dsSensorsAddresses[i][7], HEX);
    Serial.print(" ");
  }
  Serial.println("");
#endif

  sensors.begin();
  for (byte i = 0; i < DS_SENSORS_NUM; i++) {
    sensors.setResolution(dsSensorsAddresses[i], DS_TEMPERATURE_PRECISION);
    dsErrorsTemperatures[i] = 0;
    dsLastTemperatures[i] = 30;
  }
  sensors.setWaitForConversion(false);
#endif

#ifdef MY_DEBUG
  homeSystem.display_freeram();
  Serial.println("End of setup");
#endif
}

void presentation() {
// Present locally attached sensors
#ifdef MY_SENSORS_ON
  sendSketchInfo("Home Garage", "1.1");
#endif
  for (PCFRelayRecord *record : PCFRelayRecordList) {
    //here is the presentation logic
  }

  // for (PCFButtonRecord *record : PCFButtonRecordList) {

  //   for (PCF8575Home *pcf : PCF8575HomeList) {
  //     if (pcf->getAddress() == record->getPCFAddress()) {
  //       //here goes the presentation logic
  //       break;
  //     }
  //   }
  // }
}

void loop() {

#ifdef MY_DEBUG
  if (Wire.getWireTimeoutFlag()) {
    Serial.println(F("-------------> Interruption occured"));
    //TODO: update all outputs
    Wire.clearWireTimeoutFlag();
  }
#endif

  timeElapsed = millis();
  // put your main code here, to run repeatedly:

  /* 
  Funkcja używana do skanowania wejsc/wyjsc w automatyce, Oferuje interfejs w konsoli.
  Ustawić MY_SCANPCF na 1 w pliku shared.h aby wejść w ten tryb.
*/
#ifdef MY_SCANPCF
  HS.runMenu();
#endif

  //Handling IRQ event
  for (uint8_t i = 0; i < NUM_I2C_WIRES; i++) {
    if (irqWireFlag[i]) {

#ifdef MY_DEBUG
      Serial.print(F("Interruption at wire "));
      Serial.println(i);
#endif

      homeSystem.checkInputsAtLine(i);
      irqWireFlag[i] = false;
      return;
    }
  }


  for (PCFButtonRecord *record : PCFButtonRecordList) {
    record->update(&timeElapsed);
  }


  //timers checking, logic, every 200ms

  tasker.loop();

#if defined(DS18B20_ACTIVE)

#ifdef MY_SENSORS_ON
  if (DsTemperaturesToNotify) {
    //TODO:sending temperatures to HA
    //...
    DsTemperaturesToNotify = false;
  }
#endif

  if (!DsRequestedTemperatures && timeElapsed - DsRequestedAt >= DS_TEMPERATURE_INTERVAL_IN_MS) {
    DsRequestedTemperatures = true;
    DsRequestedAt = millis();

    sensors.requestTemperatures();

    for (byte i = 0; i < DS_SENSORS_NUM; i++) {
      if (dsErrorsTemperatures[i] > 2) {
#ifdef MY_DEBUG
        Serial.println("3 errors in a row. Something wrong with the sensor: ");
        Serial.println(dsSensorsAddresses[i][7], HEX);
#endif
        //TODO: Send notification to HA
      }
    }

    //digitalWrite(DS_STRONGPULLUP_PIN, HIGH);

  } else if (DsRequestedTemperatures && millis() - DsRequestedAt >= DsDelayInMs) {

    //digitalWrite(DS_STRONGPULLUP_PIN, LOW);
    DsRequestedTemperatures = false;
    for (byte i = 0; i < DS_SENSORS_NUM; i++) {
      float tempC = sensors.getTempC(dsSensorsAddresses[i]);

      if (tempC == DEVICE_DISCONNECTED_C) {
#ifdef MY_DEBUG
        Serial.print("Error: Could not read temperature data: ");
        Serial.println(dsSensorsAddresses[i][7], HEX);
#endif
        dsErrorsTemperatures[i]++;
        continue;
      }
      dsErrorsTemperatures[i] = 0;
      dsLastTemperatures[i] = tempC;
    }

    DsTemperaturesToNotify = true;
  }
#endif

  delay(30);  //smartSleep
}

#ifdef MY_SENSORS_ON
void receive(const MyMessage &message) {
// We only expect one type of message from controller. But we better check anyway.
#ifdef MY_DEBUG
  if (message.isAck()) {
    Serial.println("This is an ack from gateway");
  }
#endif

  if (message.type == V_LIGHT) {
    // Change relay state
    message.sensor bool state = message.getBool();
    digitalWrite(1, state ? 1 : 0);
    // Store state in eeprom
    //saveState(message.getSensor(), state);

    // Write some debug info
    Serial.print("Incoming change for sensor:");
    Serial.print(message.sensor);
    Serial.print(", New status: ");
    Serial.println(message.getBool());
  }
}
#endif
