//
//  https://github.com/hurzhurz/XGZP6897D
//

#include "XGZP6897D.h"

#define XGZP6897D_ADDRESS                        0x6d

XGZP6897D::XGZP6897D(uint16_t type, TwoWire *wire)
{
  _wire          = wire;
  _Kvalue        = type;
};

#if defined(ESP8266) || defined(ESP32)
bool XGZP6897D::begin(const uint8_t dataPin, const uint8_t clockPin)
{
  if ((dataPin < 255) && (clockPin < 255))
  {
    _wire->begin(dataPin, clockPin);
  } else {
    _wire->begin();
  }
  return isConnected();
}
#endif


bool XGZP6897D::begin()
{
  _wire->begin();
  return isConnected();
}

bool XGZP6897D::isConnected(uint16_t timeout)
{
  uint32_t start = micros();
  while (micros() - start < timeout)
  {
    _wire->beginTransmission(XGZP6897D_ADDRESS);
    if ( _wire->endTransmission() == 0) return true;
    yield();
    delayMicroseconds(100);
  }
  return false;
}

double XGZP6897D::getPressure()
{
  int32_t pressure = ( _data[0]<<16) + ( _data[1]<<8) + _data[2];
  if(pressure & (1<<23))
    pressure = pressure - (1<<24);
  return (double)pressure / (double)_Kvalue;
}


float XGZP6897D::getTemperature()
{
  uint32_t temperature = ( _data[3]<<8) + _data[4];
  if(temperature & (1<<15))
    temperature = temperature - (1<<16);
  return (float)temperature / 256.0;
}

bool XGZP6897D::performMeasurement()
{
  uint8_t data;
  if(!_write(0x30, 0x0a)) return false;
  delayMicroseconds(100);
  while(_read(0x30, &data))
  {
    if( !(data & 0x08) ) return true;
    delayMicroseconds(100);
  }
  return false;
}

bool XGZP6897D::continuesMeasurement(uint8_t sleeptime)
{
  return _write(0x30, (0x0b | ((sleeptime&0xf)<<4)));
}

bool XGZP6897D::stopMeasurement()
{
  return _write(0x30, 0x0);
}

bool XGZP6897D::setSampling(uint8_t sampling)
{
  uint8_t pconfig; // default: 40 / 0x28
  if(!_read(0xa6, &pconfig)) return false;
  pconfig = (pconfig & ~0x07) | (sampling&0x07);
  return _write(0xa6, pconfig);
}

bool XGZP6897D::read()
{
  return _read(0x06, _data, 5);
}

bool XGZP6897D::_read(uint8_t regaddr, uint8_t* data, uint8_t len)
{
  _wire->beginTransmission(XGZP6897D_ADDRESS);
  _wire->write(regaddr);
  if(_wire->endTransmission() != 0) return false;
  if(_wire->requestFrom(XGZP6897D_ADDRESS, len) < len) return false;
  for (int i = 0; i < len; i++)
    data[i] = _wire->read();
  return true;
}
bool XGZP6897D::_write(uint8_t regaddr, uint8_t data)
{
  _wire->beginTransmission(XGZP6897D_ADDRESS);
  _wire->write(regaddr);
  _wire->write(data);
  if(_wire->endTransmission() != 0) return false;
  return true;
}
