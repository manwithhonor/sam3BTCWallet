import serial
import time

try:
    serialcomm = serial.Serial('/dev/ttyACM1', baudrate=115200, timeout=1)
except:
    serialcomm = serial.Serial('COM3', baudrate=115200, timeout=1)
time.sleep(2)

text = "0_0"
serialcomm.write(text.encode())
time.sleep(2)
print("Response is:", serialcomm.read_all().decode('ascii'))

text = "1_m/0/1"
serialcomm.write(text.encode())
time.sleep(2)
print("Response is:", serialcomm.read_all().decode('ascii'))    

text = "1_m/0/5"
serialcomm.write(text.encode())
time.sleep(2)
print("Response is:", serialcomm.read_all().decode('ascii'))  

text = "2_m/0/1"
serialcomm.write(text.encode())
time.sleep(2)
print("Response is:", serialcomm.read_all().decode('ascii'))

text = "3_0"
serialcomm.write(text.encode())
time.sleep(2)
print("Response is:", serialcomm.read_all().decode('ascii'))

serialcomm.close()
print("END OF TEST")