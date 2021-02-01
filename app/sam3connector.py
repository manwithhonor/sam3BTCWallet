import os
import time
import json
import ecdsa
import serial
from hashlib import sha256
from prettytable import PrettyTable

def open_connection():
    print("Connecting to microcontroller...")
    try:
        serialcomm = serial.Serial('/dev/ttyACM1', baudrate=115200, timeout=1)
    except:
        serialcomm = serial.Serial('COM3', baudrate=115200, timeout=1)
    print("Connection was established.")
    print("Wait couple second for the system to start.")
    print()
    time.sleep(2)
    return serialcomm

def close_connection(serialcomm):
    serialcomm.close()

def send_request(command, data, connection):
    request = dict.fromkeys(['command', 'data'])
    request['command'] = command
    request['data'] = data
    request_str = json.dumps(request)    
    connection.write(request_str.encode())

def get_response(connection):
    response_str = connection.read_all().decode('ascii')
    # data_left = connection.in_waiting() > 0
    time.sleep(0.5)
    while connection.inWaiting() > 0:
        response_str += str(connection.read(connection.inWaiting()))
    return response_str

def parse_response(response_str):
    try:
        response_dict = json.loads(response_str)
        return response_dict
    except:
        return response_str

def cmd(command, data, connection, func_name):
    print("Sending command: {}.".format(func_name))
    send_request(command, data, connection)
    time.sleep(2)
    response_str = get_response(connection)
    response_dict = parse_response(response_str)
    return response_dict


""" 
def serial_ports():
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result
"""