#include <stdint.h>
#include <Arduino.h>
#include "./../bitcoin/Bitcoin.h"
//#include "./../platform/platform.h"

class Wallet {
    uint8_t journalTail;

public:
    Wallet();
    ~Wallet();

    void init();

    void appendJournalRecord(String record);

    void cleanJournal();
    void printJournal();

    int generateSeed();
    int readSeed(byte* seed);
    HDPrivateKey generatePrivateKey();
    PublicKey printPublicKey();
    void signTransaction(byte *hash);
};
