import os
import time
import json
import ecdsa
import serial
import sam3connector
from hashlib import sha256
from prettytable import PrettyTable

def single_pretty_print(response_dict):
    out_table = PrettyTable()
    out_table.field_names = list(response_dict.keys())
    out_table.add_row(list(response_dict.values()))
    print(out_table)

def multi_pretty_print(response_dict):
    out_table = PrettyTable()
    try:
        for key, val in response_dict.items():
            out_table.add_column(key, val)
        print(out_table)
    except:
        print(response_dict)


def pretty_output(response_dict):
    if(type(response_dict)==str):
        print (response_dict)
    elif len(response_dict) == 1:
        single_pretty_print(response_dict)
    elif len(response_dict) > 1:
        multi_pretty_print(response_dict)
    else:
        print(response_dict)

def clear_screen():
    print()
    input("Press Enter to continue...")
    sam3connector.cls()

def signature_validation(signature, hash):
    sk = ecdsa.SigningKey.generate(curve=ecdsa.SECP256k1)
    with open(os.getcwd() + r'/app/wallet.json', 'r') as transaction_file:
        keys = transaction_file.read()
        vk = sk.get_verifying_key()
    hash_bytes = str.encode(hash)
    sig = sk.sign(hash_bytes)
    
    print("Signature validation...")
    try:
        res = vk.verify(sig, hash_bytes)
    except:
        res =1
    if res:
        print("Signature is valid")
    else:
        print("ERROR: Signature is invalid")

# API
def generate_new_seed(serialcomm):
    response_dict = sam3connector.cmd("0", ["",""], serialcomm, "generateNewSeed")
    pretty_output(response_dict)
    clear_screen()
    
def get_public_keys(keyAmount, keyType, serialcomm):
    response_dict = sam3connector.cmd("1", [keyAmount, keyType], serialcomm, "getPublicKeys")
    pretty_output(response_dict)

    # response_string = json.dumps(response_dict) # dict to json
    with open(os.getcwd() + r'/app/walet.json', 'w', encoding='utf-8') as json_file:
        json.dump(response_dict, json_file, ensure_ascii=False, indent=4)

    clear_screen()

def sign_transaction(hash, derivationPath, serialcomm):
    response_dict = sam3connector.cmd("2", [hash, derivationPath], serialcomm, "signTransaction") 
    pretty_output(response_dict)
    try:
        a = response_dict['signature']
        signature_validation(response_dict, hash)
    except:
        pass
    clear_screen()

def print_journal(serialcomm):
    response_dict = sam3connector.cmd("3", ["",""], serialcomm, "printJournal")
    pretty_output(response_dict)
    clear_screen()

def clean_journal(serialcomm):
    response_dict = sam3connector.cmd("4", ["",""], serialcomm, "cleanJournal")
    pretty_output(response_dict)
    clear_screen()