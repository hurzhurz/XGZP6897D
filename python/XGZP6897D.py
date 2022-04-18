#
#  https://github.com/hurzhurz/XGZP6897D
#
#  XGZP6897D PIN layout (top view)
#  ===============================
#  
#           +---+
#   N/C  1 -|. O|- 8  GND     port1 / high pressure side
#   Vdd  2 -|   |- 7  SCL
#   N/C  3 -|   |- 6  SDA
#   N/C  4 -|O  |- 5  N/C     port2 / low pressure side
#           +---+
#
#


import smbus

class XGZP6897D:
  bus = None
  address = None
  K_value = None
  data = [0,0,0,0,0]
  
  TYPE_500PA  = 8192
  TYPE_1000PA = 4096
  TYPE_2500PA = 2048
  TYPE_5KPA   = 1024
  TYPE_10KPA  = 512
  TYPE_20KPA  = 256
  TYPE_40KPA  = 128
  TYPE_50KPA  = 128
  TYPE_100KPA = 64
  TYPE_200KPA = 32
  UNIT_C     = 0
  UNIT_K     = 1
  UNIT_F     = 2
  UNIT_PA    = 3
  UNIT_BAR   = 4
  UNIT_PSI   = 5
  UNIT_MMH2O = 6
  UNIT_MMHG  = 7
  SAMPLE_256X = 4
  SAMPLE_512X = 5
  SAMPLE_1024X = 0
  SAMPLE_2048X = 1
  SAMPLE_4096X = 2
  SAMPLE_8192X = 3
  SAMPLE_16384X = 6
  SAMPLE_32768X = 7
  
  
  def __init__(self, type ,port = 0, address = 0x6d):
    self.bus = smbus.SMBus(port)
    self.address = address
    self.K_value = int(type)
    if self.K_value < 0:
      raise ValueError

  def perform_measurement(self):
    self.bus.write_byte_data(self.address, 0x30, 0x0a)
    while (self.bus.read_byte_data(self.address, 0x30) & 0x08) > 0:
      pass

  def stop_measurement(self):
    self.bus.write_byte_data(self.address, 0x30, 0)

  def set_sampling(self,sampling):
    pconfig = self.bus.read_byte_data(self.address, 0xa6) # default: 40 / 0x28
    self.bus.write_byte_data(self.address, 0xa6, ((pconfig & ~0x07) | (sampling&0x07)) )

  def continues_measurement(self,sleeptime=0):
    self.bus.write_byte_data(self.address, 0x30, (0x0b | ((sleeptime&0xf)<<4)))

  def read(self):
    self.data = self.bus.read_i2c_block_data(self.address, 0x06, 5)

  def get_temperature(self, unit=None):
    temperature = (self.data[3]<<8) + self.data[4]
    if temperature & (1<<15):
      temperature = temperature - (1<<16)
    temperature = float(temperature) / 256.0
    if unit == self.UNIT_C or unit is None:
      return temperature
    if unit == self.UNIT_K:
      return temperature + 273.15
    if unit == self.UNIT_F:
      return temperature * 1.8 + 32
    raise Exception('undefined measurement unit')

  def get_pressure(self, unit=None):
    pressure = (self.data[0]<<16) + (self.data[1]<<8) + self.data[2]
    if pressure & (1<<23):
      pressure = pressure - (1<<24)
    pressure = float(pressure) / float(self.K_value)
    if unit == self.UNIT_PA or unit is None:
      return pressure
    if unit == self.UNIT_BAR:
      return pressure/100000.0
    if unit == self.UNIT_PSI:
      return pressure*0.00014503773773
    if unit == self.UNIT_MMH2O:
      return pressure*0.1019744288922
    if unit == self.UNIT_MMHG:
      return pressure*0.007500637554191
    raise Exception('undefined measurement unit')
