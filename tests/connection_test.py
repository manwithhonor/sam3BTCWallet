import serial
import time
import json

def send_request(command, data, connection):
    print("Sending command...")
    request = dict.fromkeys(['command', 'data'])
    request['command'] = command
    request['data'] = data
    request_str = json.dumps(request)    
    connection.write(request_str.encode())

def parse_response(response_str):
    try:
        response = json.loads(response_str)
        print('Response is: ', response)
    except:
        # print("Error occured during parsing of response string. Incoming string was:")
        print(response_str)

def open_connection():
    print("Connecting to microcontroller...")
    try:
        serialcomm = serial.Serial('/dev/ttyACM1', baudrate=115200, timeout=1)
    except:
        serialcomm = serial.Serial('COM3', baudrate=115200, timeout=1)
    print("Connection was established.")
    print("Wait couple second for the system to start.")
    time.sleep(2)
    return serialcomm

def cmd(command, data, connection):
    send_request(command, data, connection)
    time.sleep(2)    
    response_str = connection.read_all().decode('ascii')
    parse_response(response_str)
    print()

serialcomm = open_connection()

cmd("0", ["",""], serialcomm) 
cmd("1", ["2","0"], serialcomm) # direct income
cmd("1", ["2","1"], serialcomm) # exchange
cmd("2", ["dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f","m/0/1"], serialcomm)  # SHA256(Hello, World!): dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f
cmd("3", ["",""], serialcomm) 

serialcomm.close()
print("END OF TEST")