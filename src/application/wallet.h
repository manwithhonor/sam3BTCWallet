#include <stdint.h>
#include <Arduino.h>
#include "./../bitcoin/Bitcoin.h"

class Wallet {
    uint8_t journalTail;
    int readSeed(byte* seed);
    HDPrivateKey generatePrivateKey();

public:
    Wallet();
    ~Wallet();

    void init();

    void appendJournalRecord(String record);
    void cleanJournal();
    void printJournal();

    void generateSeed();
    void printPublicKey();
    void signTransaction(byte *hash, String derivationPath);
};
