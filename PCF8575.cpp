/*
 * See header file for details
 *
 *  This program is free software: you can redistribute it and/or modify\n
 *  it under the terms of the GNU General Public License as published by\n
 *  the Free Software Foundation, either version 3 of the License, or\n
 *  (at your option) any later version.\n
 * 
 *  This program is distributed in the hope that it will be useful,\n
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of\n
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n
 *  GNU General Public License for more details.\n
 * 
 *  You should have received a copy of the GNU General Public License\n
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.\n
 */

/* Dependencies */
#include <Wire.h>
#include "PCF8575.h"
#ifdef PCF8575_INTERRUPT_SUPPORT
#include "PCint.h"
#endif

PCF8575::PCF8575() :
		_PORT(0), _PIN(0), _DDR(0), _address(0)
#ifdef PCF8575_INTERRUPT_SUPPORT
		, _oldPIN(0), _isrIgnore(0), _pcintPin(0), _intMode(), _intCallback()
#endif
{
}

void PCF8575::begin(uint8_t address) {

	/* Store the I2C address and init the Wire library */
	_address = address;
	Wire.begin();
	readGPIO();
}

void PCF8575::pinMode(uint8_t pin, uint8_t mode) {

	/* Switch according mode */
	switch (mode) {
	case INPUT:
		_DDR &= ~(1 << pin);
		_PORT &= ~(1 << pin);
		break;

	case INPUT_PULLUP:
		_DDR &= ~(1 << pin);
		_PORT |= (1 << pin);
		break;
    
  case OUTPUT_PULLUP:
    _DDR |= (1 << pin);
    _PORT |= (1 << pin);
    break;
    
	case OUTPUT:
		_DDR |= (1 << pin);
		_PORT &= ~(1 << pin);
		break;

	default:
		break;
	}

	/* Update GPIO values */
	updateGPIO();
}

void PCF8575::digitalWrite(uint8_t pin, uint8_t value) {

	/* Set PORT bit value */
  Serial.println(_PORT, DEC);
	if (value)
		_PORT |= (1 << pin);
	else
		_PORT &= ~(1 << pin);
Serial.println(_PORT, DEC);
	/* Update GPIO values */
	updateGPIO();
}

uint8_t PCF8575::digitalRead(uint8_t pin) {

	/* Read GPIO */
	readGPIO();

#ifdef PCF8575_INTERRUPT_SUPPORT
	/* Check for interrupt (manual detection) */
	//checkForInterrupt();
#endif

	/* Read and return the pin state */
	return (_PIN & (1 << pin)) ? HIGH : LOW;
}

void PCF8575::write(uint16_t value) {

	/* Store pins values and apply */
	_PORT = value;

	/* Update GPIO values */
	updateGPIO();
}

uint16_t PCF8575::read() {

	/* Read GPIO */
	readGPIO();

#ifdef PCF8575_INTERRUPT_SUPPORT
	/* Check for interrupt (manual detection) */
	//checkForInterrupt();
#endif

	/* Return current pins values */
	return _PIN;
}

void PCF8575::pullUp(uint8_t pin) {

	/* Same as pinMode(INPUT_PULLUP) */
	pinMode(pin, INPUT_PULLUP); // /!\ pinMode form THE LIBRARY
}

void PCF8575::pullDown(uint8_t pin) {

	/* Same as pinMode(INPUT) */
	pinMode(pin, INPUT); // /!\ pinMode form THE LIBRARY
}

void PCF8575::clear() {

	/* User friendly wrapper for write() */
	write(0x0000);
}

void PCF8575::set() {

	/* User friendly wrapper for write() */
	write(0xFFFF);
}

void PCF8575::toggle(uint8_t pin) {

	/* Toggle pin state */
	_PORT ^= (1 << pin);

	/* Update GPIO values */
	updateGPIO();
}

void PCF8575::blink(uint8_t pin, uint16_t count, uint32_t duration) {

	/* Compute steps duration */
	duration /= count * 2;

	/* Loop n times */
	while (count--) {

		/* Toggle pin 2 times */
		toggle(pin);
		delay(duration);
		toggle(pin);
		delay(duration);
	}
}

#ifdef PCF8575_INTERRUPT_SUPPORT
void PCF8575::enableInterrupt(uint8_t pin, void (*selfCheckFunction)(void)) {

	/* Store interrupt pin number */
	_pcintPin = pin;

	/* Setup interrupt pin */
#if ARDUINO >= 100
	::pinMode(pin, INPUT_PULLUP); // /!\ pinMode form THE ARDUINO CORE
#else
	::pinMode(pin, INPUT); // /!\ pinMode form THE ARDUINO CORE
	::digitalWrite(pin, HIGH); // /!\ digitalWrite form THE ARDUINO CORE
#endif

	/* Attach interrupt handler */
	PCattachInterrupt(pin, selfCheckFunction, FALLING);
}

void PCF8575::disableInterrupt() {

	/* Detach interrupt handler */
	PCdetachInterrupt(_pcintPin);
}

void PCF8575::checkForInterrupt() {

	/* Avoid nested interrupt triggered by I2C read/write */
	if(_isrIgnore)
		return;
	else
		_isrIgnore = 1;
		
	/* Re-enable interrupts to allow Wire library to work */
	sei();

	/* Read current pins values */
	readGPIO();

	/* Check all pins */
	for (uint8_t i = 0; i < 16; ++i) {

		/* Check for interrupt handler */
		if (!_intCallback[i])
			continue;

		/* Check for interrupt event */
		switch (_intMode[i]) {
		case CHANGE:
			if ((1 << i) & (_PIN ^ _oldPIN))
				_intCallback[i]();
			break;

		case LOW:
			if (!(_PIN & (1 << i)))
				_intCallback[i]();
			break;

		case FALLING:
			if ((_oldPIN & (1 << i)) && !(_PIN & (1 << i)))
				_intCallback[i]();
			break;

		case RISING:
			if (!(_oldPIN & (1 << i)) && (_PIN & (1 << i)))
				_intCallback[i]();
			break;
		}
	}
	
	/* Turn off ISR ignore flag */
	_isrIgnore = 0;
}

void PCF8575::attachInterrupt(uint8_t pin, void (*userFunc)(void),
		uint8_t mode) {

	/* Store interrupt mode and callback */
	_intMode[pin] = mode;
	_intCallback[pin] = userFunc;
}

void PCF8575::detachInterrupt(uint8_t pin) {

	/* Void interrupt handler */
	_intCallback[pin] = 0;
}
#endif

void PCF8575::readGPIO() {

#ifdef PCF8575_INTERRUPT_SUPPORT
	/* Store old _PIN value */
	_oldPIN = _PIN;
#endif

	/* Start request, wait for data and receive GPIO values as byte */
	Wire.requestFrom(_address, (uint8_t) 0x02);
	while (Wire.available() < 2)
		;
	_PIN = I2CREAD(); /* LSB first */
	_PIN |= I2CREAD() << 8;
}

void PCF8575::updateGPIO() {

	/* Read current GPIO states */
	//readGPIO(); // Experimental

	/* Compute new GPIO states */
	//uint8_t value = ((_PIN & ~_DDR) & ~(~_DDR & _PORT)) | _PORT; // Experimental
	uint16_t value = (_PIN & ~_DDR) | _PORT;

	/* Start communication and send GPIO values as byte */
	Wire.beginTransmission(_address);
	I2CWRITE(value & 0x00FF);
	I2CWRITE((value & 0xFF00) >> 8);
	Wire.endTransmission();
}
