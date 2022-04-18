//
//  https://github.com/hurzhurz/XGZP6897D
//

#include "XGZP6897D.h"
#include "Wire.h"

XGZP6897D sensor(XGZP6897D_TYPE_50KPA, &Wire);

void setup() {
  Serial.begin(9600);
  Serial.println("XGZP6897D differential pressure sensor test");
  
  sensor.begin();
  sensor.setSampling(XGZP6897D_SAMPLE_32768X); // optional, default: 1024X ?
  
  Serial.println("");
  Serial.println("Single measurement");
  Serial.println("------------------");
  sensor.performMeasurement();
  sensor.read();
  Serial.print("Temperature: ");
  Serial.print(sensor.getTemperature());
  Serial.println(" C");
  Serial.print("Temperature: ");
  Serial.print(sensor.getTemperature_F());
  Serial.println(" F");
  Serial.print("Pressure: ");
  Serial.print(sensor.getPressure());
  Serial.println(" PA");
  Serial.print("Pressure: ");
  Serial.print(sensor.getPressure_PSI());
  Serial.println(" PSI");
  Serial.print("Pressure: ");
  Serial.print(sensor.getPressure_MMH2O());
  Serial.println(" mmH2O");
  
  Serial.println("");
  Serial.println("Continues measurement");
  Serial.println("---------------------");
  sensor.continuesMeasurement();
}

void loop() {
  delay(2000);
  sensor.read();
  Serial.print("Temperature: ");
  Serial.print(sensor.getTemperature());
  Serial.print(" / C Pressure: ");
  Serial.print(sensor.getPressure());
  Serial.println(" PA");
}
