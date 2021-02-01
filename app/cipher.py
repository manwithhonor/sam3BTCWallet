import ecdsa
from hashlib import sha256
import os
import json
from prettytable import PrettyTable


r = {'Path': 'm/0/1', 'Address': 'xpub6AXdt158Qow5WgwMTBned7tPRsqT42k7KcCRjanvmefknZt2bsXy7GEDtiVTsKHgxoEwbUy9DJwW5j4W73YGQGZaNyUwu1kmqt2hhEExtuL'}
rj = json.dumps(r) # dict to json
rr = json.loads(rj) # json to dict

with open(os.getcwd() + r'/tests/walet.json', 'w', encoding='utf-8') as json_file:
    json.dump(r, json_file, ensure_ascii=False, indent=4)

with open(os.getcwd() + r'/tests/walet.json', 'r') as json_file:    
    wallet_data = json.loads(json_file.read()) 

print(wallet_data['Address'])

"""
with open(os.getcwd() + r'/tests/transaction.txn', 'r') as transaction_file:
    transaction = transaction_file.read()

hash_sum = sha256(transaction.encode('utf-8')).hexdigest()
hash_sum_bytes = str.encode(hash_sum)

sig = '3045022100c48c8f9947f337d4eb829ccb4a7cd18acdc75fb804a9bb779dd6c8f8cda875fe0220210485e6138ba2bd5acc7c61542064c7aaf8ccfae3e362c2e54a7f770505a85f'
sig_byte = str.encode(sig)

# vk_string = 'xpub6AXdt158Qow5WgwMTBned7tPRsqT42k7KcCRjanvmefknZt2bsXy7GEDtiVTsKHgxoEwbUy9DJwW5j4W73YGQGZaNyUwu1kmqt2hhEExtuL'
vk_string = '0488b21e026738083f00000001b9e167f74ed53d3d369663fd8e7329f550613f3a4c5b2375f2e347173719d77c03013763af7a1c0afded71f861ab55eb2755e2945d4aab6d4d69f4378ce74d6c6b'
vk_string_byte = str.encode(vk_string)
vk2 = ecdsa.VerifyingKey.from_string(vk_string_byte, curve=ecdsa.SECP256k1)

#print(vk.verify(sig, hash_sum_bytes)) 
"""

"""
sk = ecdsa.SigningKey.generate(curve=ecdsa.SECP256k1) 
vk = sk.get_verifying_key()
sig = sk.sign(hash_sum_bytes)
print(vk.verify(sig, hash_sum_bytes)) # True
"""


# print (msg)
# print (sig.hex())
# print (sk.to_string().hex())
# print (vk.to_string().hex())




