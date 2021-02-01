#include "wallet.h"
#include "Hash.h"
#include "./../bitcoin/Bitcoin.h"
#include "./../../src/platform/platform.h"
#include "ArduinoJson.h"

#include "DueFlashStorage.h"
#include <ctime>

const int journalSize = 255;
const int masterKeyLength = 112;
const int seedlen = 64;
const byte magic_number[64] = "3045022100ae7a1f1cec2299e77e7a9";

Record::Record() {}

Record::Record(char user[16], char operation[32], char status[16]) {
    this->time = platform::clocks::get();
    memcpy(&this->user, user, 16 * sizeof(char));
    memcpy(&this->operation, operation, 32 * sizeof(char));
    memcpy(&this->status , status, 16 * sizeof(char));
}

void Record::print() {

    /*DynamicJsonDocument jsonOutput(1024);

    jsonOutput["signature"] = signature.toString();
    serializeJson(jsonOutput, Serial);*/

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

Wallet::Wallet() {
        byte magic[seedlen] = { 0 };
        readMagic(magic);
        if  (strcmp( (const char*) magic, (const char*) magic_number) == 0) {
            this->journalTail = readJoutnalTail();
            this->seedFlag = readSeedFlag();
        }
        else {
            this->journalTail = 0;
            this->seedFlag = 0;
            writeJoutnalTail(this->journalTail);
            writeSeedFlag(this->seedFlag);
            writeMagic( (byte*) magic_number);
        }
}
Wallet::~Wallet() {}

void Wallet::init() {}

void Wallet::writeMagic(byte* magic){
    uint32_t pointer = (journalSize + 1) * sizeof(Record);
    DueFlashStorage dueFlashStorage;
    dueFlashStorage.write(pointer, magic, seedlen);
}
int Wallet::readMagic(byte* magic){
    uint32_t pointer = (journalSize + 1)*sizeof(Record);
    platform::persistent::read(pointer, magic, seedlen);
   
    return 0; 
}

void Wallet::writeJoutnalTail(uint8_t journalTail){
    uint32_t pointer = (journalSize + 2) * sizeof(Record);
    DueFlashStorage dueFlashStorage;
    dueFlashStorage.write(pointer, journalTail);
}
uint8_t Wallet::readJoutnalTail(){
    uint8_t journalTail;
    DueFlashStorage dueFlashStorage;
    uint32_t pointer = (journalSize + 2)*sizeof(Record);
    journalTail = dueFlashStorage.read(pointer);   
    return journalTail; 
}

void Wallet::writeSeedFlag(uint8_t journalTail){
    uint32_t pointer = (journalSize + 3) * sizeof(Record);
    DueFlashStorage dueFlashStorage;
    dueFlashStorage.write(pointer, journalTail);
}
uint8_t Wallet::readSeedFlag(){
    uint8_t journalTail;
    DueFlashStorage dueFlashStorage;
    uint32_t pointer = (journalSize + 3)*sizeof(Record);
    journalTail = dueFlashStorage.read(pointer);   
    return journalTail; 
}

void Wallet::appendJournalRecord(Record record) {
    DynamicJsonDocument jsonOutput(1024);

    if (this->journalTail < journalSize) {
        uint32_t address = this->journalTail * sizeof(Record);
        byte buffer[sizeof(record)];
        memcpy(buffer, &record, sizeof(record));
        platform::persistent::write(address, buffer, sizeof(record));
        this->journalTail++;
        writeJoutnalTail(this->journalTail);
    } else {
        jsonOutput["Message"] = "Journal is full!";
        serializeJson(jsonOutput, Serial);
    }
}

void Wallet::printJournal() {
    DynamicJsonDocument jsonOutput(1024);
    if (this->journalTail == 0) {
        jsonOutput["Message"] = "Journal is empty!";
        serializeJson(jsonOutput, Serial);
    } else {
        uint32_t address;
        Record record;
        byte temp[sizeof(Record)];
        DynamicJsonDocument jsonOutput(1024);

        for (uint32_t j = 0; j < this->journalTail; j++) {
            address = j*sizeof(Record);
            platform::persistent::read(address, temp, sizeof(Record));
            memcpy(&record, temp, sizeof(Record));

            jsonOutput["Datetime"][j] = String(record.time.hours) + String(":") + String(record.time.minutes) + String(":")+ String(record.time.seconds) + String(" ") + String(record.time.day) + String(".") + String(record.time.month) + String(".") + String(record.time.year);
            jsonOutput["User"][j] = String(record.user);
            jsonOutput["Operation"][j] = String(record.operation);
            jsonOutput["Status"][j] = String(record.status);
        }
        serializeJson(jsonOutput, Serial);
    }
}

void Wallet::cleanJournal() {
    DynamicJsonDocument jsonOutput(1024);

    jsonOutput["Message"] = "Journal has been cleaned!";
    serializeJson(jsonOutput, Serial);

    Record flood;
    flood.time = platform::clocks::Time{0, 0, 0, 0, 0, 0};
    char user[16] =  {""};
    char operation[32] = {""};
    char status[16] = {""};

    memcpy(&flood.user, user, 16 * sizeof(char));
    memcpy(&flood.operation, operation, 32 * sizeof(char));
    memcpy(&flood.status , status, 16 * sizeof(char));

    byte buffer[sizeof(Record)];
    memcpy(buffer, &flood, sizeof(flood));

    uint32_t address;
    for (uint32_t i = 0; i < this->journalTail; i++) {
        address = i * sizeof(Record);
        platform::persistent::write(address, buffer, sizeof(flood));
    }

    this->journalTail = 0;
    writeJoutnalTail(this->journalTail);

}

void Wallet::generateSeed(){
    char cStrHex[65] = {0};
    int seedLen = 0;
    char cc[] = "Bitcoin seed";
    byte seed[seedlen] = { 0 };
    DynamicJsonDocument jsonOutput(1024);

    srand(this->journalTail); 
    //srand(0); // Set seed for randomizer

    for(int i=0 ; i < seedlen; i++){
        sprintf(cStrHex+i, "%x", rand() % 16); // Fill the char buffer
    }
    
    seedLen = sha512Hmac((byte*)cc, strlen(cc), (byte*)cStrHex, strlen(cStrHex), seed);
    uint32_t pointer = (journalSize + 4) * sizeof(Record);
    platform::persistent::write(pointer, seed, seedlen);
    this->seedFlag = 1;
    writeSeedFlag(1);

    jsonOutput["Message"] = "Seed was created and saved.";
    serializeJson(jsonOutput, Serial);

    
    /*
    for (int i = 0; i < 256; i++){                       // here you have to get big 256-digital number
        long_numer = long_number + digitalRead(PB21)*(2^i);     // on board with AT91SAM3U input with random signal is 
    }                                                         // on the pin PB21. 
    */
}

int Wallet::readSeed(byte* seed){
    uint32_t pointer = (journalSize + 4)*sizeof(Record);
    platform::persistent::read(pointer, seed, seedlen);
   
    return 0; 
}

HDPrivateKey Wallet::generatePrivateKey() {
    HDPrivateKey hd;
    byte seed[seedlen] = { 0 };
    Wallet wallet;
    wallet.readSeed(seed);
    hd.setSecret((uint8_t*) seed);
    return hd;
}

void Wallet::printPublicKeys(int keyAmount, int keyType) {
    DynamicJsonDocument jsonOutput(1024);
    Wallet wallet;
    HDPrivateKey masterkey = wallet.generatePrivateKey();
    HDPublicKey pubKey = masterkey.xpub();
    String derivationPath;

    for(int i=0; i < keyAmount; i++){
        derivationPath = String("m/") + keyType + String("/") + i;
        
        jsonOutput["Derivation path"][i] = String(derivationPath);        
        jsonOutput["Public key"][i] = pubKey.derive(derivationPath).serialize();        
        // Serial.println(pubKey.derive(derivationPath).serialize()); ///xpub
    };
    serializeJson(jsonOutput, Serial);

}

void Wallet::signTransaction(byte *hash, String derivationPath) {
    DynamicJsonDocument jsonOutput(1024);
    Wallet wallet;
    HDPrivateKey masterkey = wallet.generatePrivateKey();
    PrivateKey privkey = masterkey.derive(derivationPath);
    Signature signature = privkey.sign(hash);

    jsonOutput["signature"] = signature.toString();
    serializeJson(jsonOutput, Serial);
}