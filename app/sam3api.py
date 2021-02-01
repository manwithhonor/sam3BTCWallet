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
    for key, val in response_dict.items():
        out_table.add_column(key, val)
    print(out_table)

# API
def generate_new_seed(serialcomm):
    response_dict = sam3connector.cmd("0", ["",""], serialcomm, "generateNewSeed")
    single_pretty_print(response_dict)
    
def get_public_keys(keyAmount, keyType, serialcomm):
    response_dict = sam3connector.cmd("1", [keyAmount, keyType], serialcomm, "getPublicKeys")
    multi_pretty_print(response_dict)

def sign_transaction(hash, derivationPath, serialcomm):
    response_dict = sam3connector.cmd("2", [hash, derivationPath], serialcomm, "signTransaction") 
    single_pretty_print(response_dict)

def print_journal(serialcomm):
    response_dict = sam3connector.cmd("3", ["",""], serialcomm, "printJournal")
    multi_pretty_print(response_dict)

def clean_journal(serialcomm):
    response_dict = sam3connector.cmd("4", ["",""], serialcomm, "cleanJournal")
    single_pretty_print(response_dict)