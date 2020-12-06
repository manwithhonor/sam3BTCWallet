#include <stdint.h>
#include <Arduino.h>

#include "./../platform/platform.h"

class Wallet {
    uint8_t journalTail;

public:
    Wallet();
    ~Wallet();

    void init();

    void appendJournalRecord(String record);

    void cleanJournal();
    void printJournal();

    void generatePrivateKey();
    void printPublicKey();
    void signTransaction();
};
