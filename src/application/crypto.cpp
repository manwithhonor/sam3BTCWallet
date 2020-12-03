#include "crypto.h"
#include "./../platform/platform.h"
#include "Bitcoin.h"
#include "Hash.h"
#include "journal.h"
// #include "./platform/sam3X/DueFlashStorage.h"

#define StringLength    (256/4)




uint16_t long_numer = 0;      //256 - digital number to generate private master key "010011101010..."

namespace crypto {
    namespace generate {
        int randomInt(byte* seed) {
            
            char cStrHex[(StringLength+1)] = {0};
            int seedLen = 0;
            char cc[] = "Bitcoin seed";

            srand(0); // Seed random:
            // Fill the char buffer
            for(int i=0 ; i < StringLength; i++){
                sprintf(cStrHex+i, "%x", rand() % 16);
            }

            seedLen = sha512Hmac((byte*)cc, strlen(cc), (byte*)cStrHex, strlen(cStrHex), seed);
            // Serial.println("Random number1: " + toHex(seed, seedLen));
            // platfrom::console::printString("Random number: " + toHex(mySeed, 64));
            return 0;

            /*
            for (uint16_t i = 0; i < 256; i++){                       // here you have to get big 256-digital number
            long_numer = long_number + digitalRead(PB21)*(2^i);     // on board with AT91SAM3U input with random signal is 
            }                                                         // on the pin PB21. 
            
           return 0;*/
        }
    }
}


int getHash(String transaction, byte* hash, int hashlen) {  
    if (hashlen < 64) {
        return 1;
    }

    char cc[] = "Bitcoin seed";
    int written = sha512Hmac((byte*)cc, strlen(cc), (byte*)transaction.c_str(), transaction.length(), hash);
    // Serial.println("Sha512-HMAC: " + toHex(hash, written));
    return written < hashlen ? 1 : 0;
} 
  
HDPrivateKey genMasterPrivateKey(byte *seed) {
    HDPrivateKey hd("ABCDEFG09");
    hd.setSecret((uint8_t*) seed);          // put here your 256-digit number, but it should be in string format, not integer!
    return hd;
} 

HDPublicKey getMasterPublicKey(HDPrivateKey hd) {
    HDPublicKey hd_pub = hd.xpub();
    return hd_pub;
}
  
PrivateKey getChildPrivateKey(HDPrivateKey hd, int N) {  
    PrivateKey child_private_key = hd.child(N);    //that all is just a random using of derivation functions
    return child_private_key;
}

PublicKey getChildPublicKey(PrivateKey child_private_key) {
    PublicKey child_public_key = child_private_key.publicKey();
    return child_public_key;
}
 
Signature signTransaction(PrivateKey child_private_key, byte *hash) {
    Signature signature = child_private_key.sign(hash);
    return signature;
}

void write_master_key2(byte *seed) { 
    //Serial.println("seed beffore  written!: " + toHex(seed, 64));
    uint32_t pointer = (MAXIMUM_SIZE + 1)*sizeof(Record);
    //byte buffer[sizeof(seed)];
    //memcpy(buffer, &seed, sizeof(seed) );

    // dueFlashStorage.write(pointer, seed, 64);
    platfrom::persistent::write(pointer, seed, 64);
    Serial.println("seed has been written!"); 
}


void print_master_key2() {
    uint32_t pointer = (MAXIMUM_SIZE + 1)*sizeof(Record);
    // byte * b = dueFlashStorage.readAddress(pointer);
    byte * b = platfrom::persistent::read(pointer);
    byte temp[64];
    memcpy(&temp, b, 64);
    Serial.println("Key2: " + toHex(temp, 64));
}

void write_master_key(char * key) {
    uint16_t pointer = (MAXIMUM_SIZE + 1)*sizeof(Record);
    // dueFlashStorage.write(pointer, (byte *)key, MASTER_KEY_LENGTH); 
    platfrom::persistent::write(pointer, (byte *)key, MASTER_KEY_LENGTH);
    Serial.println("Master key has been written!");
}

void print_master_key() {
    uint16_t pointer = (MAXIMUM_SIZE + 1)*sizeof(Record);
    // byte * b = dueFlashStorage.readAddress(pointer);
    byte * b = platfrom::persistent::read(pointer);
    char temp[MASTER_KEY_LENGTH];
    memcpy(&temp, b, MASTER_KEY_LENGTH);
    Serial.println("This is the master key:");
    Serial.println(temp);
}
