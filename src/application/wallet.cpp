#include "wallet.h"
#include "Hash.h"
#include "./../bitcoin/Bitcoin.h"
#include "./../../src/platform/platform.h"

#include "../../platform/sam3x/DueFlashStorage.h"

const int journalSize = 1000;
const int masterKeyLength = 112;

struct Record {
    platform::clocks::Time time;
    char user[16];
    char status[16];
    char operation[32];

    Record();
    Record(char user[16], char operation[32], char status[16]);
    void print();
};

Record::Record(char user[16], char operation[32], char status[16]) {
    Record record;
    record.time = platform::clocks::get();
    memcpy(&record.user, user, 16 * sizeof(char));
    memcpy(&record.operation, operation, 32 * sizeof(char));
    memcpy(&record.status, status, 16 * sizeof(char));
}

void Record::print() {
    Serial.print("Time: ");
    Serial.print(this->time.hours);
    Serial.print(":");
    Serial.print(this->time.minutes);
    Serial.print(":");
    Serial.print(this->time.seconds);
    Serial.print(" ");
    Serial.print(this->time.day);
    Serial.print(".");
    Serial.print(this->time.month);
    Serial.print(".");
    Serial.print(this->time.year);
    Serial.print(", User: ");
    Serial.print(this->user);
    Serial.print(", Operation: ");
    Serial.print(this->operation);
    Serial.print(", Status: ");
    Serial.print(this->status);
    Serial.println("");
}

Wallet::Wallet() {}
Wallet::~Wallet() {}

void Wallet::init() {}

void Wallet::appendJournalRecord(String record) {
    if (this->journalTail < journalSize) {
        Record record = Record();

        uint32_t address = this->journalTail * sizeof(Record);
        byte buffer[sizeof(record)];
        memcpy(buffer, &record, sizeof(record));
        platform::persistent::write(address, buffer, sizeof(record));
        this->journalTail++;
        Serial.println("Record has been written!");
    } else {
        Serial.println("Journal is full!");
    }
}

void Wallet::cleanJournal() {
    Record flood;
    flood.time = platform::clocks::Time{0, 0, 0, 0, 0, 0};

    byte buffer[sizeof(Record)];
    memcpy(buffer, &flood, sizeof(flood));

    uint32_t address;
    for (uint32_t i = 0; i < journalSize; i++) {
        address = i * sizeof(Record);
        platform::persistent::write(address, buffer, sizeof(flood));
    }

    this->journalTail = 0;
    Serial.println("Journal has been cleaned!");
}

void Wallet::printJournal() {
    if (this->journalTail == 0) {
        Serial.println("Journal is empty!");
    } else {
        Serial.println("--------------------Begin journal--------------------");
        for (uint32_t j = 0; j < this->journalTail; j++) {
            /*uint16_t i;
            i = j*sizeof(Record);
            byte* b = platform::persistent::read(i);
            Record record;
            memcpy(&record, b, sizeof(Record));
            record.print();*/
        }
        Serial.println("--------------------End journal----------------------");
    }
}


int Wallet::generateSeed(){
    char cStrHex[65] = {0};
    int seedLen = 0;
    char cc[] = "Bitcoin seed";
    byte seed[64] = { 0 };
    //srand((unsigned int) time(0)); 
    srand(0); // Set seed for randomizer

    
    for(int i=0 ; i < 64; i++){
        sprintf(cStrHex+i, "%x", rand() % 16); // Fill the char buffer
    }
    
    seedLen = sha512Hmac((byte*)cc, strlen(cc), (byte*)cStrHex, strlen(cStrHex), seed);
    Serial.println("Seed: " + toHex(seed, seedLen));

    uint16_t pointer = (journalSize + 1) * sizeof(Record);
    platform::persistent::write(pointer, seed, 64);
    Serial.println("Seed has been written.");


    DueFlashStorage dueFlashStorage;
    byte * b = dueFlashStorage.readAddress(pointer);
    byte temp[64]= { 0 };
    platform::persistent::read(pointer, temp);
    //byte temp[64];
    //memcpy(&temp, b, 64);
    Serial.println("TEST3: " + toHex(temp, 64));


    return 0; 
    
    /*
    for (int i = 0; i < 256; i++){                       // here you have to get big 256-digital number
        long_numer = long_number + digitalRead(PB21)*(2^i);     // on board with AT91SAM3U input with random signal is 
    }                                                         // on the pin PB21. 
    */
}

int Wallet::readSeed(byte* seed){
    Serial.println("Start");
    uint32_t pointer = (journalSize + 1)*sizeof(Record);
    //platform::persistent::read(pointer, seed);
    Serial.println("mid");
    DueFlashStorage dueFlashStorage;
    byte * b = dueFlashStorage.readAddress(pointer);
    byte temp[64];
    memcpy(&temp, b, 64);
    // byte seed1[64];
    // memcpy(&seed1, seed, 64);
    Serial.println("Key2: " + toHex(temp, 64));
    
    return 0; 
}

HDPrivateKey Wallet::generatePrivateKey() {
    HDPrivateKey hd("ABCDEFG09");
    // HDPrivateKey hd();
    byte seed[64] = { 0 }; // this seed should be random generated
    Wallet wallet;
    wallet.readSeed(seed);
    hd.setSecret((uint8_t*) seed);
    return hd;

    /*uint16_t pointer = (journalSize + 1) * sizeof(Record);
    platform::persistent::write(pointer, (byte *)hd, masterKeyLength);
    Serial.println("Master key has been written!");*/
}

PublicKey Wallet::printPublicKey() {
   /* byte seed[64] = { 0 }; // this seed should be random generated
    Wallet wallet;
    wallet.readSeed(seed);
    Serial.println("Seed: " + toHex(seed, 64));

    HDPrivateKey hd("ABCDEFG09");
    hd.setSecret((uint8_t*) seed);
    Serial.print("This is master private key: ");
    Serial.println(hd);

    PublicKey pubKey = hd.publicKey();
    Serial.print("This is master public key: ");
    Serial.println(pubKey);
    return pubKey;*/
}

void Wallet::signTransaction(byte *hash) {
    /*byte seed[64] = { 0 }; // this seed should be random generated
    Wallet wallet;
    wallet.readSeed(seed);
    Serial.println("Seed: " + toHex(seed, 64));

    HDPrivateKey hd();
    hd.setSecret((uint8_t*) seed);
    Serial.print("This is master private key: ");
    Serial.println(pubKey);

    Signature signature = hd.sign(hash);
    Serial.print("Signature: ");
    Serial.println(signature);*/
}
