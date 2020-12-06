#include "wallet.h"
#include "Hash.h"
#include "./../bitcoin/Bitcoin.h"
#include "./../../src/platform/platform.h"

const int journalSize = 1000;
const int masterKeyLength = 112;
const int seedlen = 64;

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


void Wallet::generateSeed(){
    char cStrHex[65] = {0};
    int seedLen = 0;
    char cc[] = "Bitcoin seed";
    byte seed[seedlen] = { 0 };
    //srand((unsigned int) time(0)); 
    srand(0); // Set seed for randomizer

    
    for(int i=0 ; i < seedlen; i++){
        sprintf(cStrHex+i, "%x", rand() % 16); // Fill the char buffer
    }
    
    seedLen = sha512Hmac((byte*)cc, strlen(cc), (byte*)cStrHex, strlen(cStrHex), seed);
    Serial.println("Seed: " + toHex(seed, seedLen));

    uint32_t pointer = (journalSize + 1) * sizeof(Record);
    platform::persistent::write(pointer, seed, seedlen);
    Serial.println("Seed has been written.");

    
    /*
    for (int i = 0; i < 256; i++){                       // here you have to get big 256-digital number
        long_numer = long_number + digitalRead(PB21)*(2^i);     // on board with AT91SAM3U input with random signal is 
    }                                                         // on the pin PB21. 
    */
}

int Wallet::readSeed(byte* seed){
    uint32_t pointer = (journalSize + 1)*sizeof(Record);
    platform::persistent::read(pointer, seed, seedlen);
   
    return 0; 
}

HDPrivateKey Wallet::generatePrivateKey() {
    HDPrivateKey hd;
    byte seed[seedlen] = { 0 };
    Wallet wallet;
    wallet.readSeed(seed);
    hd.setSecret((uint8_t*) seed);
    // Serial.println(hd);
    return hd;
}

void Wallet::printPublicKey() {
    Wallet wallet;
    HDPrivateKey masterkey = wallet.generatePrivateKey();
    Serial.print("This is master private key: ");
    Serial.println(masterkey);

    HDPublicKey pubKey = masterkey.xpub();
    Serial.print("This is master public key: ");
    Serial.println(pubKey);

    String derivationPath;

    // get plain adresesses
    for(int i=0; i<20; i++){
        derivationPath = String("m/0/") + i;
        Serial.print("Path: " + derivationPath + ", ");
        Serial.print("Address: ");
        Serial.println(pubKey.derive(derivationPath).address());
    }

    // get adresesses for change
    for(int i=0; i<10; i++){
        derivationPath = String("m/1/") + i;
        Serial.print("Path: " + derivationPath + ", ");
        Serial.print("Address: ");
        Serial.println(pubKey.derive(derivationPath).address());
    }

}

void Wallet::signTransaction(byte *hash, String derivationPath) {
    Wallet wallet;
    HDPrivateKey masterkey = wallet.generatePrivateKey();
    PrivateKey privkey = masterkey.derive(derivationPath);

    Serial.print("This is master private key: ");
    Serial.println(masterkey);
    Serial.print("This is child private key: ");
    Serial.println(privkey);

    Signature signature = privkey.sign(hash);
    Serial.print("Signature: ");
    Serial.println(signature);
}
