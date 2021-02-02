import os
import time
import json
import ecdsa
import serial

import sam3api
import sam3connector

from hashlib import sha256
from prettytable import PrettyTable

def isint(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

title =(
"""
  __           _   _                                     
 (_   _. ._ _  _) /  ._   ._ _|_  _ \    / _. | |  _ _|_ 
 __) (_| | | | _) \_ | \/ |_) |_ (_) \/\/ (_| | | (/_ |_ 
                       /  |                              
"""   )                                                            
print(title)

menu = PrettyTable()
menu.add_column("Command", [1,2,3,4,5, '', 0], align="r")
menu.add_column("Descryption", ['Generate new master seed', 'Get public keys', 'Sign transaction', 'Print journal', 'Clean journal', ' ', 'Exit'], align="l")

serialcomm = sam3connector.open_connection()

while True:
    print(menu.get_string(title="Menu"))
    print()

    scenario = input("Enter number of command: ")
    if isint(scenario):
        scenario = int(scenario)
    else:
        print('Invalid command')
        pass
    
    if scenario == 1:    
        sam3api.generate_new_seed(serialcomm)

    elif scenario == 2:
        while 1:
            amount = input("Enter required amount of keys (integer number): ")
            if isint(amount):
                break
            else:
                print('Wrong number')
        while 1:
            key_type = input("Enter required type of keys (0 - for direct pays, 1 - for change): ")
            if isint(amount):
                if int(key_type) == 0 or int(key_type) == 1:
                    break
                else:
                    print('Wrong number')
            else:
                print('Wrong number')

        sam3api.get_public_keys(amount, key_type, serialcomm) 
    elif scenario == 3:
        with open(os.getcwd() + r'/app/transaction.txn', 'r') as transaction_file:
            transaction = transaction_file.read()
        hash_sum = sha256(transaction.encode('utf-8')).hexdigest()

        transaction_dict = json.loads(transaction)
        transaction_table = PrettyTable()
        transaction_table.add_column("Fields", list(transaction_dict.keys()))
        transaction_table.add_column("Vaues", list(transaction_dict.values()))
        print(transaction_table.get_string(title="Transaction"))
        print()

        der_path = str(input("Enter derivation path in format 'm/0/1': "))
        sam3api.sign_transaction(str(hash_sum), der_path, serialcomm)
        

    elif scenario == 4:
        sam3api.print_journal(serialcomm)
    elif scenario == 5:
        sam3api.clean_journal(serialcomm)
    elif scenario == 0:                
        sam3connector.close_connection(serialcomm)
        break
    else:
        print('Invalid command')
        print()
        pass

end_title = (
"""
  _                _    _                        
 |_ ._   _|    _ _|_   |_) ._ _   _  ._ _. ._ _  
 |_ | | (_|   (_) |    |   | (_) (_| | (_| | | | 
                                  _|             
""")
print(end_title)