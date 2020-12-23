#include <stdint.h>
#include <Arduino.h>
#include "./../bitcoin/Bitcoin.h"
#include "./../../src/platform/platform.h"

struct Record {
    platform::clocks::Time time;
    char user[16];
    char status[16];
    char operation[32];

    Record();
    Record(char user[16], char operation[32], char status[16]);
    void print();
};


class Wallet {
    uint8_t journalTail = 0;
   

    void writeJoutnalTail(uint8_t journalTail);
    uint8_t readJoutnalTail();

    int readSeed(byte* seed);
    HDPrivateKey generatePrivateKey();

public:
    Wallet();
    ~Wallet();
    uint8_t seedFlag = 0;
    void init();
    
    void writeSeedFlag(uint8_t journalTail);
    uint8_t readSeedFlag();

    void appendJournalRecord(Record record);
    void cleanJournal();
    void printJournal();

    void generateSeed();
    void printPublicKeys(int keyAmount, int keyType);
    void signTransaction(byte *hash, String derivationPath);
};
