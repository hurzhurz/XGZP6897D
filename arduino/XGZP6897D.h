#pragma once
//
//  https://github.com/hurzhurz/XGZP6897D
//
//  XGZP6897D PIN layout (top view)
//  ===============================
//  
//           +---+
//   N/C  1 -|. O|- 8  GND     port1 / high pressure side
//   Vdd  2 -|   |- 7  SCL
//   N/C  3 -|   |- 6  SDA
//   N/C  4 -|O  |- 5  N/C     port2 / low pressure side
//           +---+
//
//


#include "Arduino.h"
#include "Wire.h"


#define XGZP6897D_TYPE_500PA  8192
#define XGZP6897D_TYPE_1000PA 4096
#define XGZP6897D_TYPE_2500PA 2048
#define XGZP6897D_TYPE_5KPA   1024
#define XGZP6897D_TYPE_10KPA  512
#define XGZP6897D_TYPE_20KPA  256
#define XGZP6897D_TYPE_40KPA  128
#define XGZP6897D_TYPE_50KPA  128
#define XGZP6897D_TYPE_100KPA 64
#define XGZP6897D_TYPE_200KPA 32
#define XGZP6897D_SAMPLE_256X   4
#define XGZP6897D_SAMPLE_512X   5
#define XGZP6897D_SAMPLE_1024X  0
#define XGZP6897D_SAMPLE_2048X  1
#define XGZP6897D_SAMPLE_4096X  2
#define XGZP6897D_SAMPLE_8192X  3
#define XGZP6897D_SAMPLE_16384X 6
#define XGZP6897D_SAMPLE_32768X 7


class XGZP6897D
{
public:

  XGZP6897D(uint16_t type, TwoWire *wire = &Wire);

#if defined(ESP8266) || defined(ESP32)
  bool     begin(const uint8_t dataPin, const uint8_t clockPin);
#endif
  bool     begin();

  bool     isConnected(uint16_t timeout = 3000);

  bool performMeasurement();
  bool continuesMeasurement(uint8_t sleeptime = 0);
  bool stopMeasurement();
  bool setSampling(uint8_t sampling);
  bool read();
  
  double getPressure();
  float getTemperature();
  
  double getPressure_PA()    { return getPressure(); }
  double getPressure_BAR()   { return getPressure()/100000.0; }
  double getPressure_PSI()   { return getPressure()*0.00014503773773; }
  double getPressure_MMH2O() { return getPressure()*0.1019744288922; }
  double getPressure_MMHG()  { return getPressure()*0.007500637554191; }
  
  float getTemperature_C() { return getTemperature(); }
  float getTemperature_K() { return getTemperature() + 273.15; }
  float getTemperature_F() { return getTemperature() * 1.8 + 32.0; }
 

private:

  bool     _read(uint8_t regaddr, uint8_t* data, uint8_t len=1);
  bool     _write(uint8_t regaddr, uint8_t data);

  TwoWire* _wire;
  uint16_t _Kvalue;
  uint8_t  _data[5];
};
