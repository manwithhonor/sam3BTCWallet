import serial
import time
from statistics import mean

try:
    serialcomm = serial.Serial('/dev/ttyACM1', baudrate=115200, timeout=1)
except:
    serialcomm = serial.Serial('COM3', baudrate=115200, timeout=1)
time.sleep(2)

def cmd(request, connection):
    connection.write(request.encode())
    time.sleep(1.7)
    response = connection.read_all().decode('ascii')

def time_test(request, connection):
    time_list = []
    for i in range(50):
        start_time = time.time()
        cmd(request, connection) 
        time_list.append(time.time() - start_time)
    return mean(time_list)

seed_time = time_test("0_0", serialcomm)
key_time = time_test("1_m/0/1", serialcomm)
sign_time = time_test("2_m/0/1", serialcomm)
journal_time = time_test("3_0", serialcomm)

print(seed_time)
print(key_time)
print(sign_time)
print(journal_time)

serialcomm.close()
print("END OF TEST")


"""def cmd(cmd,serial):
    out='';prev='101001011'
    serial.flushInput();serial.flushOutput()
    serial.write(cmd+'\r');
    while True:
        out+= str(serial.read(1))
        if prev == out: return out
        prev=out
    return out
    
    cmd('ATZ',serial.Serial('/dev/ttyUSB0', timeout=1, baudrate=115000))    

#Modified code from main loop: 
s = serial.Serial(5)

#Modified code from thread reading the serial port
while 1:
  tdata = s.read()           # Wait forever for anything
  time.sleep(1)              # Sleep (or inWaiting() doesn't give the correct value)
  data_left = s.inWaiting()  # Get the number of characters ready to be read
  tdata += s.read(data_left) # Do the read and combine it with the first character

   #Rest of the code

    """