import serial
import time
import json

def time_decorator(fn):
    def wrapped(*args, **kwargs):
        start_time = time.time()
        res = fn(*args, **kwargs)
        delta = time.time() - start_time - 2
        print("Execution time of the function is {:.3f} sec.".format(delta))
        print()
        return res
    return wrapped

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
        # print(response['response'])
        return response['response']
    except:
        # print(response_str)
        return response_str


@time_decorator
def cmd(command, data, connection, func_name):
    print("Sending command: {}.".format(func_name))
    send_request(command, data, connection)
    time.sleep(2)    
    response_str = get_response(connection)
    response = parse_response(response_str)
    # print(response)
    return response

# API
def generate_new_seed(serialcomm):
    response = cmd("0", ["",""], serialcomm, "generateNewSeed")
    print(response)
    
def get_public_keys(keyAmount, keyType, serialcomm):
    response = cmd("1", [keyAmount, keyType], serialcomm, "getPublicKeys")
    print(response)

def sign_transaction(hash, derivationPath, serialcomm):
    response = cmd("2", [hash, derivationPath], serialcomm, "signTransaction") 
    print(response)

def print_journal(serialcomm):
    response = cmd("3", ["",""], serialcomm, "printJournal") 
    return response

def clean_journal(serialcomm):
    response = cmd("4", ["",""], serialcomm, "cleanJournal")
    print(response)

def deleteSeed(serialcomm):
    response = cmd("5", ["",""], serialcomm, "deleteSeed") 
    print(response)

def make_overload(serialcomm):
    response = cmd("6", ["",""], serialcomm, "makeOverload") 
    print(response)


# SHA256(Hello, World!): dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f 
test_hash = "dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f"
test_path = "m/0/1"
serialcomm = open_connection()

while True:
    scenario = int(input("Enter number of scenario: "))

    if scenario == 1:
        deleteSeed(serialcomm)
        get_public_keys(2, 0, serialcomm) # direct income
        response = print_journal(serialcomm)        
        for row in response.split('\n')[-3:-2]:
            print(row)
    elif scenario == 2:
        generate_new_seed(serialcomm)
        get_public_keys(-5, 1, serialcomm)
        get_public_keys(2.5, 1, serialcomm)
        get_public_keys("random string", 1, serialcomm)
        get_public_keys(0, 1, serialcomm)
        response = print_journal(serialcomm)        
        for row in response.split('\n')[-7:-2]:
            print(row)
    elif scenario == 3:
        deleteSeed(serialcomm)
        sign_transaction(test_hash, test_path, serialcomm) 
        response = print_journal(serialcomm)        
        for row in response.split('\n')[-3:-2]:
            print(row)
    elif scenario == 4:
        generate_new_seed(serialcomm)
        sign_transaction(test_hash, "M", serialcomm)
        sign_transaction(test_hash, "256", serialcomm) 
        sign_transaction(test_hash, "m//1/0/1", serialcomm)  
        sign_transaction(test_hash, "random string", serialcomm)  
        response = print_journal(serialcomm)        
        for row in response.split('\n')[-7:-2]:
            print(row)
    elif scenario == 5:
        make_overload(serialcomm)
        generate_new_seed(serialcomm)
        get_public_keys(2, 0, serialcomm) # direct income
        sign_transaction(test_hash, test_path, serialcomm) # Success
    else:
        break

serialcomm.close()
print("END OF TEST")