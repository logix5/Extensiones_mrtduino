/*
  DS1302.cpp - Arduino library support for the DS1302 Trickle Charge Timekeeping Chip
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library has been made to easily interface and use the DS1302 RTC with
  the Arduino.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/
#include "DS1302.h"

#define REG_SEC		0
#define REG_MIN		1
#define REG_HOUR	2
#define REG_DATE	3
#define REG_MON		4
#define REG_DOW		5
#define REG_YEAR	6
#define REG_WP		7
#define REG_TCR		8

/* Public */

Time::Time()
{
	this->year = 2010;
	this->mon  = 1;
	this->date = 1;
	this->hour = 0;
	this->min  = 0;
	this->sec  = 0;
	this->dow  = 5;
}

DS1302_RAM::DS1302_RAM()
{
	for (int i=0; i<31; i++)
		cell[i]=0;
}

DS1302::DS1302(uint8_t ce_pin, uint8_t data_pin, uint8_t sclk_pin)
{
	_ce_pin = ce_pin;
	_data_pin = data_pin;
	_sclk_pin = sclk_pin;

	pinMode(_ce_pin, OUTPUT);
	pinMode(_sclk_pin, OUTPUT);
}

Time DS1302::getTime()
{
	Time t;
	_burstRead();
	t.sec	= _decode(_burstArray[0]);
	t.min	= _decode(_burstArray[1]);
	t.hour	= _decodeH(_burstArray[2]);
	t.date	= _decode(_burstArray[3]);
	t.mon	= _decode(_burstArray[4]);
	t.dow	= _burstArray[5];
	t.year	= _decodeY(_burstArray[6])+2000;
	return t;
}

void DS1302::setTime(uint8_t hour, uint8_t min, uint8_t sec)
{
	if (((hour>=0) && (hour<24)) && ((min>=0) && (min<60)) && ((sec>=0) && (sec<60)))
	{
		_writeRegister(REG_HOUR, _encode(hour));
		_writeRegister(REG_MIN, _encode(min));
		_writeRegister(REG_SEC, _encode(sec));
	}
}

void DS1302::setDate(uint8_t date, uint8_t mon, uint16_t year)
{
	if (((date>0) && (date<=31)) && ((mon>0) && (mon<=12)) && ((year>=2000) && (year<3000)))
	{
		year -= 2000;
		_writeRegister(REG_YEAR, _encode(year));
		_writeRegister(REG_MON, _encode(mon));
		_writeRegister(REG_DATE, _encode(date));
	}
}

void DS1302::setDOW(uint8_t dow)
{
	if ((dow>0) && (dow<8))
		_writeRegister(REG_DOW, dow);
}

char *DS1302::getTimeStr(uint8_t format)
{
	char *output= "xxxxxxxx";
	Time t;
	t=getTime();
	if (t.hour<10)
		output[0]=48;
	else
		output[0]=char((t.hour / 10)+48);
	output[1]=char((t.hour % 10)+48);
	output[2]=58;
	if (t.min<10)
		output[3]=48;
	else
		output[3]=char((t.min / 10)+48);
	output[4]=char((t.min % 10)+48);
	output[5]=58;
	if (format==FORMAT_SHORT)
		output[5]=0;
	else
	{
	if (t.sec<10)
		output[6]=48;
	else
		output[6]=char((t.sec / 10)+48);
	output[7]=char((t.sec % 10)+48);
	output[8]=0;
	}
	return output;
}

char *DS1302::getDateStr(uint8_t slformat, uint8_t eformat, char divider)
{
	char *output= "xxxxxxxxxx";
	int yr, offset;
	Time t;
	t=getTime();
	switch (eformat)
	{
		case FORMAT_LITTLEENDIAN:
			if (t.date<10)
				output[0]=48;
			else
				output[0]=char((t.date / 10)+48);
			output[1]=char((t.date % 10)+48);
			output[2]=divider;
			if (t.mon<10)
				output[3]=48;
			else
				output[3]=char((t.mon / 10)+48);
			output[4]=char((t.mon % 10)+48);
			output[5]=divider;
			if (slformat==FORMAT_SHORT)
			{
				yr=t.year-2000;
				if (yr<10)
					output[6]=48;
				else
					output[6]=char((yr / 10)+48);
				output[7]=char((yr % 10)+48);
				output[8]=0;
			}
			else
			{
				yr=t.year;
				output[6]=char((yr / 1000)+48);
				output[7]=char(((yr % 1000) / 100)+48);
				output[8]=char(((yr % 100) / 10)+48);
				output[9]=char((yr % 10)+48);
				output[10]=0;
			}
			break;
		case FORMAT_BIGENDIAN:
			if (slformat==FORMAT_SHORT)
				offset=0;
			else
				offset=2;
			if (slformat==FORMAT_SHORT)
			{
				yr=t.year-2000;
				if (yr<10)
					output[0]=48;
				else
					output[0]=char((yr / 10)+48);
				output[1]=char((yr % 10)+48);
				output[2]=divider;
			}
			else
			{
				yr=t.year;
				output[0]=char((yr / 1000)+48);
				output[1]=char(((yr % 1000) / 100)+48);
				output[2]=char(((yr % 100) / 10)+48);
				output[3]=char((yr % 10)+48);
				output[4]=divider;
			}
			if (t.mon<10)
				output[3+offset]=48;
			else
				output[3+offset]=char((t.mon / 10)+48);
			output[4+offset]=char((t.mon % 10)+48);
			output[5+offset]=divider;
			if (t.date<10)
				output[6+offset]=48;
			else
				output[6+offset]=char((t.date / 10)+48);
			output[7+offset]=char((t.date % 10)+48);
			output[8+offset]=0;
			break;
		case FORMAT_MIDDLEENDIAN:
			if (t.mon<10)
				output[0]=48;
			else
				output[0]=char((t.mon / 10)+48);
			output[1]=char((t.mon % 10)+48);
			output[2]=divider;
			if (t.date<10)
				output[3]=48;
			else
				output[3]=char((t.date / 10)+48);
			output[4]=char((t.date % 10)+48);
			output[5]=divider;
			if (slformat==FORMAT_SHORT)
			{
				yr=t.year-2000;
				if (yr<10)
					output[6]=48;
				else
					output[6]=char((yr / 10)+48);
				output[7]=char((yr % 10)+48);
				output[8]=0;
			}
			else
			{
				yr=t.year;
				output[6]=char((yr / 1000)+48);
				output[7]=char(((yr % 1000) / 100)+48);
				output[8]=char(((yr % 100) / 10)+48);
				output[9]=char((yr % 10)+48);
				output[10]=0;
			}
			break;
	}
	return output;
}

char *DS1302::getDOWStr(uint8_t format)
{
	char *output= "xxxxxxxxx";
	Time t;
	t=getTime();
	switch (t.dow)
	{
		case MONDAY:
			output="Lunes";
			break;
		case TUESDAY:
			output="Martes";
			break;
		case WEDNESDAY:
			output="Miércoles";
			break;
		case THURSDAY:
			output="Jueves";
			break;
		case FRIDAY:
			output="Viernes";
			break;
		case SATURDAY:
			output="Sábado";
			break;
		case SUNDAY:
			output="Domingo";
			break;
	}     
	if (format==FORMAT_SHORT)
		output[3]=0;
	return output;
}

char *DS1302::getMonthStr(uint8_t format)
{
	char *output= "xxxxxxxxx";
	Time t;
	t=getTime();
	switch (t.mon)
	{
		case 1:
			output="Enero";
			break;
		case 2:
			output="Febrero";
			break;
		case 3:
			output="Marzo";
			break;
		case 4:
			output="Abril";
			break;
		case 5:
			output="Mayo";
			break;
		case 6:
			output="Junio";
			break;
		case 7:
			output="Julio";
			break;
		case 8:
			output="Agosto";
			break;
		case 9:
			output="Septiembre";
			break;
		case 10:
			output="Octubre";
			break;
		case 11:
			output="Noviembre";
			break;
		case 12:
			output="Diciembre";
			break;
	}     
	if (format==FORMAT_SHORT)
		output[3]=0;
	return output;
}

void DS1302::halt(bool enable)
{
  uint8_t _reg = _readRegister(REG_SEC);
  _reg &= ~(1 << 7);
  _reg |= (enable << 7);
  _writeRegister(REG_SEC, _reg);
}

void DS1302::writeProtect(bool enable)
{
  uint8_t _reg = (enable << 7);
  _writeRegister(REG_WP, _reg);
}

void DS1302::setTCR(uint8_t value)
{
	_writeRegister(REG_TCR, value);
}

/* Private */

uint8_t DS1302::_readByte()
{
	pinMode(_data_pin, INPUT);

	uint8_t value = 0;
	uint8_t currentBit = 0;

	for (int i = 0; i < 8; ++i)
	{
		currentBit = digitalRead(_data_pin);
		value |= (currentBit << i);
		digitalWrite(_sclk_pin, HIGH);
		delayMicroseconds(1);
		digitalWrite(_sclk_pin, LOW);
	}
	return value;
}

void DS1302::_writeByte(uint8_t value)
{
	pinMode(_data_pin, OUTPUT);
	shiftOut(_data_pin, _sclk_pin, LSBFIRST, value);
}

uint8_t DS1302::_readRegister(uint8_t reg)
{
	uint8_t cmdByte = 129;
	cmdByte |= (reg << 1);

	uint8_t readValue;

	digitalWrite(_sclk_pin, LOW);
	digitalWrite(_ce_pin, HIGH);

	_writeByte(cmdByte);
	readValue = _readByte();
	
	digitalWrite(_ce_pin, LOW);

	return readValue;
}

void DS1302::_writeRegister(uint8_t reg, uint8_t value)
{
	uint8_t cmdByte = (128 | (reg << 1));

	digitalWrite(_sclk_pin, LOW);
	digitalWrite(_ce_pin, HIGH);

	_writeByte(cmdByte);
	_writeByte(value);

	digitalWrite(_ce_pin, LOW);
}

void DS1302::_burstRead()
{
	digitalWrite(_sclk_pin, LOW);
	digitalWrite(_ce_pin, HIGH);

	_writeByte(191);
	for (int i=0; i<8; i++)
	{
		_burstArray[i] = _readByte();
	}
	digitalWrite(_ce_pin, LOW);
}

uint8_t	DS1302::_decode(uint8_t value)
{
	uint8_t decoded = value & 127;
	decoded = (decoded & 15) + 10 * ((decoded & (15 << 4)) >> 4);
	return decoded;
}

uint8_t DS1302::_decodeH(uint8_t value)
{
  if (value & 128)
    value = (value & 15) + (12 * ((value & 32) >> 5));
  else
    value = (value & 15) + (10 * ((value & 48) >> 4));
  return value;
}

uint8_t	DS1302::_decodeY(uint8_t value)
{
	uint8_t decoded = (value & 15) + 10 * ((value & (15 << 4)) >> 4);
	return decoded;
}

uint8_t DS1302::_encode(uint8_t value)
{
	uint8_t encoded = ((value / 10) << 4) + (value % 10);
	return encoded;
}

void DS1302::writeBuffer(DS1302_RAM r)
{
	digitalWrite(_sclk_pin, LOW);
	digitalWrite(_ce_pin, HIGH);

	_writeByte(254);
	for (int i=0; i<31; i++)
	{
		_writeByte(r.cell[i]);
	}
	digitalWrite(_ce_pin, LOW);
}

DS1302_RAM DS1302::readBuffer()
{
	DS1302_RAM r;

	digitalWrite(_sclk_pin, LOW);
	digitalWrite(_ce_pin, HIGH);

	_writeByte(255);
	for (int i=0; i<31; i++)
	{
		r.cell[i] = _readByte();
	}
	digitalWrite(_ce_pin, LOW);

	return r;
}

void DS1302::poke(uint8_t addr, uint8_t value)
{
	if ((addr >=0) && (addr<=30))
	{
		addr = (addr * 2) + 192;

		digitalWrite(_sclk_pin, LOW);
		digitalWrite(_ce_pin, HIGH);

		_writeByte(addr);
		_writeByte(value);

		digitalWrite(_ce_pin, LOW);
	}
}

uint8_t DS1302::peek(uint8_t addr)
{
	if ((addr >=0) && (addr<=30))
	{
		addr = (addr * 2) + 193;

		uint8_t readValue;

		digitalWrite(_sclk_pin, LOW);
		digitalWrite(_ce_pin, HIGH);

		_writeByte(addr);
		readValue = _readByte();
		
		digitalWrite(_ce_pin, LOW);

		return readValue;
	}
	else
		return 0;
}

