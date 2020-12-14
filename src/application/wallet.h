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
    uint8_t journalTail=0;
    int readSeed(byte* seed);
    HDPrivateKey generatePrivateKey();

public:
    Wallet();
    ~Wallet();

    void init();

    void appendJournalRecord(Record record);
    void cleanJournal();
    void printJournal();

    void generateSeed();
    void printPublicKey(String derivationPath);
    void signTransaction(byte *hash, String derivationPath);
};
