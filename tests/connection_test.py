import serial
import time
import json

print("Connecting to microcontroller...")
try:
    serialcomm = serial.Serial('/dev/ttyACM1', baudrate=115200, timeout=1)
except:
    serialcomm = serial.Serial('COM3', baudrate=115200, timeout=1)
print("Connection was established.")
print("Wait couple second for the system to start.")
time.sleep(2)

def cmd(command, data, connection):
    request = dict.fromkeys(['command', 'data'])
    request['command'] = command
    request['data'] = data
    request_str = json.dumps(request)
    
    print("Sending command...")
    connection.write(request_str.encode())
    time.sleep(1.7)    
    response_str = connection.read_all().decode('ascii')
    
    try:
        response = json.loads(response_str)
        print(response)
        print('Response is: ', response['data'])
    except:
        try:
            if response_str.startswith('null'):
                response_str = response_str.replace('null', '')
            response = json.loads(response_str)
            print(response['data'])
        except:
            print("Error occured during parsing of response string. Incoming string was:")
            print(response_str)
    

"""request = dict.fromkeys(['command', 'data'])
request['command'] = 0
request['data'] = 0
print (request)"""
cmd("0", "0", serialcomm) 

# {"command":0,"data":0}

# cmd("0_0", serialcomm) 
"""cmd("1_m/0/1", serialcomm)
cmd("1_m/0/5", serialcomm)
cmd("2_m/0/1", serialcomm)
cmd("3_0", serialcomm)"""

serialcomm.close()
print("END OF TEST")