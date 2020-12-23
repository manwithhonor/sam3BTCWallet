import serial
import time
import json

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
        response = json.loads(response_str)
        return [0, response]
        #print('Response is: ', response)
    except:
        # print("Error occured during parsing of response string. Incoming string was:")
        print(response_str)
        return [1, response_str]

def cmd(command, data, connection):
    send_request(command, data, connection)
    time.sleep(2)    
    response_str = get_response(connection)
    response = parse_response(response_str)
    return response

# API
def generate_new_seed(serialcomm):
    print("Sending command: generateNewSeed.")
    response = cmd("0", ["",""], serialcomm) 
    if response[0] == 0:
        print(response[1]['text'])
    print()
    

def get_public_keys(keyAmount, keyType, serialcomm):
    print("Sending command: getPublicKeys.")
    response = cmd("1", [keyAmount, keyType], serialcomm)
    if response[0] == 0:
        print(response[1]['text'])
    print()

def sign_transaction(hash, derivationPath, serialcomm):
    print("Sending command: signTransaction.")
    response = cmd("2", [hash, derivationPath], serialcomm) 
    if response[0] == 0:
        print("Siganture: ", response[1]['signature'])
    print()

def print_journal(serialcomm):
    print("Sending command: printJournal.")
    response = cmd("3", ["",""], serialcomm) 
    if response[0] == 0:
        print(response[1]['text'])
    print()

def clean_journal(serialcomm):
    print("Sending command: printJournal.")
    response = cmd("4", ["",""], serialcomm) 
    if response[0] == 0:
        print(response[1]['text'])
    print()

def deleteseed(serialcomm):
    print("Sending command: deleteSeed.")
    response = cmd("5", ["",""], serialcomm) 
    if response[0] == 0:
        print(response[1]['text'])
    print()

scenario = int(input("Enter number of scenario: "))

if scenario == 1:
    serialcomm = open_connection()
    deleteseed(serialcomm)
    get_public_keys(2, 0, serialcomm) # direct income
    print_journal(serialcomm)
elif scenario == 2:
    serialcomm = open_connection()
    generate_new_seed(serialcomm)
    get_public_keys(-5, 1, serialcomm)
    get_public_keys(2.5, 1, serialcomm)
    get_public_keys("random string", 1, serialcomm)
    get_public_keys(0, 1, serialcomm)
    print_journal(serialcomm)
elif scenario == 3:
    serialcomm = open_connection()
    deleteseed(serialcomm)
    sign_transaction("dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f", "m/0/1", serialcomm) 
    print_journal(serialcomm)
elif scenario == 4:
    serialcomm = open_connection()
    generate_new_seed(serialcomm)
    sign_transaction("dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f", "M", serialcomm)
    sign_transaction("dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f", "256", serialcomm) 
    sign_transaction("dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f", "m//1/0/1", serialcomm)  
    sign_transaction("dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f", "random string", serialcomm)  
    print_journal(serialcomm)
else:
    print("wrong number")
    print("END OF TEST")

serialcomm.close()
print("END OF TEST")