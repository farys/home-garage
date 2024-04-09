#ifndef HomeAddressIO_h
#define HomeAddressIO_h

class HomeAddressIO {
public:

  HomeAddressIO(){};

  HomeAddressIO(uint8_t _ioAddress) {
    HomeAddressIO::decodeAddress(_ioAddress, *this);
  }

  HomeAddressIO(uint8_t wireLine, uint8_t pcfAddress, uint8_t pcfPort) {
    this->wireLine = wireLine;
    this->pcfAddress = pcfAddress;
    this->pcfPort = pcfPort;
  }

  //two bytes for line number
  //two bytes for pcf address
  //four bytes for port number

  static uint8_t encodeAddress(uint8_t wireLine, uint8_t pcfAddress, uint8_t pcfPort) {
    //#ifdef MY_DEBUG
    // Serial.println("koduje adress: ");
    // Serial.println(wireLine);
    // Serial.println(pcfAddress);
    // Serial.println(pcfPort);
    //#endif
    return (wireLine << 6) | ((pcfAddress - 32) << 4) | pcfPort;
  }

  static void decodeAddress(uint8_t ioAddress, HomeAddressIO &addressObject) {
    addressObject.pcfPort = HomeAddressIO::decodePortAddress(ioAddress);
    addressObject.pcfAddress = (0b00000011 & (ioAddress >> 4)) + 32;
    addressObject.wireLine = ioAddress >> 6;
  }

  static uint8_t decodePortAddress(uint8_t ioAddress) {
    return 0b00001111 & ioAddress;
  }

  uint8_t getAddressEncoded() {
    return HomeAddressIO::encodeAddress(this->wireLine, this->pcfAddress, this->pcfPort);
  }

  uint8_t getWireLine() {
    return this->wireLine;
  }

  uint8_t getPcfAddress() {
    return this->pcfAddress;
  }

  uint8_t getPcfPort() {
    return this->pcfPort;
  }

  void printDetails() {
    Serial.print(F("(PCF details [line: "));
    Serial.print(this->getWireLine());
    Serial.print(F(", address: "));
    Serial.print(this->getPcfAddress());
    Serial.print(F(", port: "));
    Serial.print(this->getPcfPort());

    Serial.println(F("])"));
  }

protected:
  uint8_t wireLine;  //i2c line
  uint8_t pcfAddress;
  uint8_t pcfPort;
};

#endif
