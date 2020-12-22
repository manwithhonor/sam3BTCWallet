import json

"""# from dict to json (string)
blog = {'URL': 'datacamp.com', 'name': 'Datacamp'}
to_json = json.dumps(blog)

# from json string to dict
hui = '{"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}'
to_python = json.loads(hui)
print(to_python.keys())"""

import serial
import time

try:
    serialcomm = serial.Serial('/dev/ttyACM1', baudrate=115200, timeout=1)
except:
    serialcomm = serial.Serial('COM3', baudrate=115200, timeout=1)
time.sleep(2)

def cmd(request, connection):
    connection.write(request.encode())
    time.sleep(2)
    print("Response is:", connection.read_all().decode('ascii'))

cmd('{"command":0,"data":["",""]}', serialcomm) 
cmd('{"command":1,"data":["",""]}', serialcomm) 
cmd('{"command":2,"data":["dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f","m/0/1"]}', serialcomm) 
# cmd('{"command":2,"data":{"hash":"dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f","derivationPath":"m/0/1"}}', serialcomm) 

serialcomm.close()
print("END OF TEST")