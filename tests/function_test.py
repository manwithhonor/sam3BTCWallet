import serial
import time
import json

def time_decorator(fn):
    def wrapped(*args, **kwargs):
        start_time = time.time()
        fn(*args, **kwargs)
        delta = time.time() - start_time - 2
        print("Execution time of the function is {:.3f} sec.".format(delta))
        print()
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
        print(response['response'])
        return response['response']
    except:
        print(response_str)
        return response_str


@time_decorator
def cmd(command, data, connection, func_name):
    print("Sending command: {}.".format(func_name))
    send_request(command, data, connection)
    time.sleep(2)    
    response_str = get_response(connection)
    response = parse_response(response_str)
    return response

# API
def generate_new_seed(serialcomm):
    response = cmd("0", ["",""], serialcomm, "generateNewSeed") 
    
def get_public_keys(keyAmount, keyType, serialcomm):
    response = cmd("1", [keyAmount, keyType], serialcomm, "getPublicKeys")

def sign_transaction(hash, derivationPath, serialcomm):
    response = cmd("2", [hash, derivationPath], serialcomm, "signTransaction") 

def print_journal(serialcomm):
    response = cmd("3", ["",""], serialcomm, "printJournal") 

def clean_journal(serialcomm):
    response = cmd("4", ["",""], serialcomm, "cleanJournal") 

def deleteSeed(serialcomm):
    response = cmd("5", ["",""], serialcomm, "deleteSeed") 

def make_overload(serialcomm):
    response = cmd("6", ["",""], serialcomm, "makeOverload") 


# SHA256(Hello, World!): dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f 
test_hash = "dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f"
test_path = "m/0/1"
serialcomm = open_connection()

while True:
    scenario = int(input("Enter number of scenario: "))

    if scenario == 1:    
        generate_new_seed(serialcomm)
    elif scenario == 2:
        # generate_new_seed(serialcomm)
        get_public_keys(2, 0, serialcomm) 
    elif scenario == 3:
        # generate_new_seed(serialcomm)
        sign_transaction(test_hash, test_path, serialcomm) 
    elif scenario == 4:
        generate_new_seed(serialcomm)
        get_public_keys(2, 0, serialcomm) 
        sign_transaction(test_hash, test_path, serialcomm) 
    elif scenario == 5:
        print_journal(serialcomm)
    elif scenario == 6:
        clean_journal(serialcomm)
        time.sleep(1.5)
        print_journal(serialcomm)
    else:
        break

serialcomm.close()
print("END OF TEST")


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