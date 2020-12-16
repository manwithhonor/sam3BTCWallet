import serial
import time

try:
    serialcomm = serial.Serial('/dev/ttyACM1', baudrate=115200, timeout=1)
except:
    serialcomm = serial.Serial('COM3', baudrate=115200, timeout=1)
time.sleep(2)

def cmd(request, connection):
    connection.write(request.encode())
    time.sleep(1.7)
    print("Response is:", connection.read_all().decode('ascii'))

cmd("0_0", serialcomm) 
cmd("1_m/0/1", serialcomm)
cmd("1_m/0/5", serialcomm)
cmd("2_m/0/1", serialcomm)
cmd("3_0", serialcomm)

serialcomm.close()
print("END OF TEST")