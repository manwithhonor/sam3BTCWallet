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
        print('Response is: ', response)
    except:
        try:
            if response_str.startswith('null'):
                response_str = response_str.replace('null', '')
            response = json.loads(response_str)
            print('Response is: ', response)
        except:
            print("Error occured during parsing of response string. Incoming string was:")
            print(response_str)
    

# Hello, World!
# SHA256 checksum: dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f

cmd("0", ["",""], serialcomm) 
cmd("1", ["",""], serialcomm) 
cmd("2", ["dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f","m/0/1"], serialcomm) 
cmd("3", ["",""], serialcomm) 

serialcomm.close()
print("END OF TEST")