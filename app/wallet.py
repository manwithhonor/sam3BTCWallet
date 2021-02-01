import os
import time
import json
import ecdsa
import serial

import sam3api
import sam3connector

from hashlib import sha256
from prettytable import PrettyTable

title =(
"""
  __           _   _                                     
 (_   _. ._ _  _) /  ._   ._ _|_  _ \    / _. | |  _ _|_ 
 __) (_| | | | _) \_ | \/ |_) |_ (_) \/\/ (_| | | (/_ |_ 
                       /  |                              
"""   )                                                            
print(title)

menu = PrettyTable()
menu.add_column("Command", list(range(1,6)), align="r")
menu.add_column("Descryption", ['Generate new master seed', 'Get public keys', 'Sign transaction', 'Print journal', 'Clean journal'], align="l")


########################################
# SHA256(Hello, World!): dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f
with open(os.getcwd() + r'/app/transaction.txn', 'r') as transaction_file:
    transaction = transaction_file.read()

hash_sum = sha256(transaction.encode('utf-8')).hexdigest()
# hash_sum_bytes = str.encode(hash_sum)
# test_hash = "dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f"
test_path = "m/0/1"
########################################

serialcomm = sam3connector.open_connection()

while True:
    print(menu)
    print()
    scenario = int(input("Enter number of command: "))

    if scenario == 1:    
        sam3api.generate_new_seed(serialcomm)
    elif scenario == 2:
        sam3api.get_public_keys(2, 0, serialcomm) 
    elif scenario == 3:
        sam3api.sign_transaction(str(hash_sum), test_path, serialcomm) 
    elif scenario == 4:
        sam3api.print_journal(serialcomm)
    elif scenario == 5:
        sam3api.clean_journal(serialcomm)
    else:        
        sam3connector.close_connection(serialcomm)
        break

end_title = (
"""
  _                _    _                        
 |_ ._   _|    _ _|_   |_) ._ _   _  ._ _. ._ _  
 |_ | | (_|   (_) |    |   | (_) (_| | (_| | | | 
                                  _|             
""")
print(end_title)





"""
with open(os.getcwd() + r'/tests/transaction.txn', 'r') as transaction_file:
    transaction = transaction_file.read()

hash_sum = sha256(transaction.encode('utf-8')).hexdigest()
hash_sum_bytes = str.encode(hash_sum)
print(hash_sum)

sk = ecdsa.SigningKey.generate(curve=ecdsa.SECP256k1) 
vk = sk.get_verifying_key()
# msg = b"message"
sig = sk.sign(hash_sum_bytes)
print(vk.verify(sig, hash_sum_bytes)) # True)

"""



