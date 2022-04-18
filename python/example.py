#
#  https://github.com/hurzhurz/XGZP6897D
#

from XGZP6897D import XGZP6897D
import time

mysensor = XGZP6897D(XGZP6897D.TYPE_50KPA,1)
mysensor.set_sampling(XGZP6897D.SAMPLE_32768X) # optional, default: 1024X ?

print('Single measurement')
print('------------------')
mysensor.perform_measurement()
mysensor.read()
print('Temperature:',mysensor.get_temperature(),'C')
print('Temperature:',mysensor.get_temperature(XGZP6897D.UNIT_F),'F')
print('Pressure:',mysensor.get_pressure(),'PA')
print('Pressure:',mysensor.get_pressure(XGZP6897D.UNIT_PSI),'PSI')
print('Pressure:',mysensor.get_pressure(XGZP6897D.UNIT_MMH2O),'mmH2O')

print('')
print('Continues measurement')
print('---------------------')
mysensor.continues_measurement()
for x in range(0, 5):
  time.sleep(1)
  mysensor.read()
  print('Temperature:',mysensor.get_temperature(),'C','Pressure:',mysensor.get_pressure(),'PA')
mysensor.stop_measurement()
