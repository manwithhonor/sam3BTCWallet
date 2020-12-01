#include "./../platform/platform.h"
#include "./../bitcoin/Bitcoin.h"

#define MAXIMUM_SIZE 1000       //max size of storage
#define MASTER_KEY_LENGTH 112   //length of master key

namespace crypto {
    namespace generate {
        int randomInt(platfrom::string seed);
    }
}

/*
int getHash(String transaction, byte* hash, int hashlen);

HDPublicKey getMasterPublicKey(HDPrivateKey hd);
PrivateKey getChildPrivateKey(HDPrivateKey hd, int N);
PublicKey getChildPublicKey(PrivateKey child_private_key);
Signature signTransaction(PrivateKey child_private_key, byte *hash);

void write_master_key2(byte *seed);
void print_master_key2();
void write_master_key(char * key);
void print_master_key();
*/