#ifndef HomeScanner_h
#define HomeScanner_h

#include "shared.h"
#include "Bonezegei_List.h"
#include "HomeSystemIO.h"


class HomeScanner {
public:
  bool running = false;

  void runMenu() {

    if (this->running == true) return;

    int menu = 0;
    Serial.println(F("[MENU] Tryb skanowania:"));
    Serial.println(F("1. Wykryj rozgałęźniki"));
    Serial.println(F("2. Wykryj przycisk"));
    Serial.println(F("3. Wykryj przekaźnik/wyjście"));
    Serial.println(F("4. Włącz/Wyłącz przekaźnik/wyjście"));
    Serial.println(F("5. Wykryj wszystkie czujniki"));
    Serial.println(F("6. Pobierz temperatury czujnikow"));

    Serial.print(F("Wpisz numer wyboru: "));
    while (Serial.available() <= 0) {}
    menu = Serial.parseInt();
    Serial.println(F("\nWybrano "));
    Serial.println(menu, DEC);

    switch (menu) {
      case 0:
        break;
      case -1:
        break;

      case 1:
        runPcfDiscovery();
        break;
      case 2:
        runInputDiscovery();
        break;

      case 3:
        runRelayDiscovery();
        break;

      case 4:
        toggleRelay();
        break;

      case 5:
        runDSDiscovery();
        break;

      case 6:
        runDSRead();
        break;
    }
    this->running = true;
  };

  void runDSDiscovery() {
    Serial.println(F("Odpytywanie wszystkich czujnikow"));
    uint8_t address[8];
    uint8_t count = 0;


    if (oneWire.search(address)) {
      Serial.println(F("[][8] = {"));
      do {
        count++;
        Serial.print("  {");
        for (uint8_t i = 0; i < 8; i++) {
          Serial.print(F("0x"));
          if (address[i] < 0x10) Serial.print(F("0"));
          Serial.print(address[i], HEX);
          if (i < 7) Serial.print(", ");
        }
        Serial.println("  },");
      } while (oneWire.search(address));

      Serial.println(F("};"));
      Serial.print(F("// nr devices found: "));
      Serial.println(count);
    } else {
      Serial.println(F("Not found"));
    }

    while (Serial.available() <= 0) { delay(20); }
    uint8_t _ioAddress;
    _ioAddress = Serial.parseInt();
  }

  void runDSRead() {
    Serial.println(F("Odczytywanie temperatury z wszystkich skonfigurowanych czujnikow"));
    sensors.setWaitForConversion(true);

    while (1) {
      sensors.requestTemperatures();

      for (int i = 0; i < DS_SENSORS_NUM; i++) {
        for (uint8_t y = 0; y < 8; y++) {
          // zero pad the address if necessary
          if (dsSensorsAddresses[i][y] < 16) Serial.print(F("0"));
          Serial.print(dsSensorsAddresses[i][y], HEX);
        }
        Serial.print(F(" -> "));
        float tempC = sensors.getTempC(dsSensorsAddresses[i]);
        if (tempC == DEVICE_DISCONNECTED_C) {
          Serial.println(F("Error: Could not read temperature data"));
          continue;  //return;
        }
        Serial.print(F("Temp C: "));
        Serial.println(tempC);
      }
      Serial.println("-----------------");
      delay(10000);
    }
  }
  void toggleRelay() {
    Serial.println(F("Musi być lista PCF ustawiona przedtem"));
    Serial.println(F("Podaj adres przekaźnika. Jeśli się nie włącza to możliwe że to włącznik!: "));
    while (true) {
      while (Serial.available() <= 0) { delay(20); }
      uint8_t _ioAddress;
      _ioAddress = Serial.parseInt();

      Serial.print("Włączam przekaznik: ");
      Serial.println(_ioAddress);

      homeSystem.asOutput(_ioAddress);
      delay(20);
      homeSystem.turnOn(_ioAddress);

      Serial.println("Włączony.");
    }
  }
  void runInputDiscovery() {
    Serial.println(F("Musi być lista PCF ustawiona przedtem"));
    Serial.println(F("Ustawiam wszystkie podłączenia jako włączniki i nasluchuje kliknięć/zwarć sygnałowych"));
    //wszystkie porty PCF na input_pullup nic nie powinno sie stac
    //przekazniki bedzie 3.3v z 3.3v po obu stronach
    PCFButtonRecord *_button[NUM_I2C_WIRES * PCF_MAX_ON_LINE * 16];

    for (PCF8575Home *pcf : PCF8575HomeList) {

#ifdef MY_DEBUG
      Serial.print(F("Konfiguruje PCF: linia: "));
      Serial.print(pcf->getLine());
      Serial.print(F(", adres: "));
      Serial.println(pcf->getAddress());
#endif

      uint8_t _pcfAddress = pcf->getAddress();
      int8_t _wireLine = pcf->getLine();
      for (uint8_t i = 0; i < 16; i++) {

        uint8_t _ioAddress = HomeAddressIO::encodeAddress(_wireLine, _pcfAddress, i);
        //Serial.println(String(_ioAddress));
        _button[_ioAddress] = new PCFButtonRecord(_ioAddress);

#ifdef MY_DEBUG
        Serial.print(F("--> Ustawiam jako przycisk: "));
        Serial.println(_ioAddress, DEC);
#endif
        homeSystem.asInput(_ioAddress);
        pcf->attachButton(HomeAddressIO::decodePortAddress(_ioAddress), _button[_ioAddress]);
      }
    }
    homeSystem.display_freeram();
    Serial.println(F("Zrestartuj sterownik aby przestać nasłuchiwać"));
  };

  void runRelayDiscovery() {
    Serial.println(F("Musi być lista PCF ustawiona przedtem"));

    uint8_t allUsedIO[NUM_I2C_WIRES * PCF_MAX_ON_LINE * 16];
    for (int i = 0, n = NUM_I2C_WIRES * PCF_MAX_ON_LINE * 16; i < n; i++) { allUsedIO[i] = 0; }

    /*
        Lista przekaznikow/wlacznikow ktorych sprawdzanie mozna pominac bo sa juz ustawione
        jesli nie maja byc pomijane to zakomentowac te dwie ponizsze petle
      */

    for (PCFRelayRecord *record : PCFRelayRecordList) {
      allUsedIO[record->getIOAddress()] = 1;
    }

    for (PCFButtonRecord *record : PCFButtonRecordList) {
      allUsedIO[record->getIOAddress()] = 1;
    }

    int r = 1;

    for (PCF8575Home *pcf : PCF8575HomeList) {
      Serial.println(pcf->getLine());
      for (byte i = 0; i < 16; i += r) {
        uint8_t _ioAddress = HomeAddressIO::encodeAddress(pcf->getLine(), pcf->getAddress(), i);
        if (allUsedIO[_ioAddress] == 0) {
          Serial.print(F("--> Włączam wyjście pod adresem "));
          Serial.println(_ioAddress);
          delay(500);
          PCFRelayRecord _relay(_ioAddress);
          _relay.configure();
          delay(500);
          _relay.setOn();

          // HomeSystemIO::turnOn(_ioAddress);
          Serial.print(F("Wyślij 1 aby przejść do kolejnego wyjścia"));

          delay(500);

          while (Serial.available() <= 0) { delay(20); }
          r = Serial.parseInt();

          Serial.print(F("--> Wyłączam wyjście pod adresem "));
          Serial.println(_ioAddress);
          _relay.setOff();
          //HomeSystemIO::turnOff(_ioAddress);
        }
      }
    }

    Serial.println(F("Wszystkie potencjalne wyjścia sprawdzone"));
    Serial.println(F("Zrestartuj sterownik"));
  };

  void runPcfDiscovery() {
    Serial.println(F("1. Wyszukiwanie rozgałęźników..."));

    for (byte i = 8; i < 127; i++) {
      Wire.beginTransmission(i);
      if (Wire.endTransmission() == 0) {

        Serial.print(F("Znaleziono nadrozgałęźnik pod adresem: "));
        Serial.println(i, DEC);
        break;
      }
      delay(1);
    }
    Wire.endTransmission();

    Serial.println(F("Przechodze do sprawdzania linii"));

    for (byte l = 0; l < NUM_I2C_WIRES; l++) {
      Serial.print(F("Sprawdzam linie "));
      Serial.println(l);
      delay(2);
#ifdef TCAADDR
      homeSystem.setActiveWireLine(l);
#endif
      //searching for pcfs line 0
      for (uint8_t i = 32; i < 40; i++) {

        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0) {

          // check address
          if ((i >= 32) && (i <= 40)) {
            // found device, do something..
            Serial.print(F("Znaleziono pod adresem IO: "));
            Serial.print(HomeAddressIO::encodeAddress(l, i, 0), DEC);
            Serial.print(F(" [pcf address: "));
            Serial.println(i, BIN);

            // Serial.print(F("Nowe IOAddress od: "));
            // Serial.print(String(HomeAddressIO::encodeAddress(0, i, 0)));
            // Serial.print(F(" do: "));
            // Serial.println(String(HomeAddressIO::encodeAddress(0, i, 15)));
          }
          delay(1);
        }
      }
    }
    Wire.endTransmission();
    Serial.println(F("Koniec sprawdzania..."));
  }

  static void scannerPrintClick(void *ioAddress) {
    Serial.print(F("--> Wykryto klikniecie pod adresem: "));
    uint8_t *_ioAddress = (uint8_t *)ioAddress;
    Serial.println(*_ioAddress);
  }
};

#endif
