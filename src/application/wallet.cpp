#include "wallet.h"
#include "./../bitcoin/Bitcoin.h"

const int journalSize = 1000;
const int masterKeyLength = 112;

struct Record {
    platfrom::clocks::Time time;
    char user[16];
    char status[16];
    char operation[32];

    Record();
    Record(char user[16], char operation[32], char status[16]);
    void print();
};

Record::Record(char user[16], char operation[32], char status[16]) {
    Record record;
    record.time = platfrom::clocks::get();
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
        platfrom::persistent::write(address, buffer, sizeof(record));
        this->journalTail++;
        Serial.println("Record has been written!");
    } else {
        Serial.println("Journal is full!");
    }
}

void Wallet::cleanJournal() {
    Record flood;
    flood.time = platfrom::clocks::Time{0, 0, 0, 0, 0, 0};

    byte buffer[sizeof(Record)];
    memcpy(buffer, &flood, sizeof(flood));

    uint32_t address;
    for (uint32_t i = 0; i < journalSize; i++) {
        address = i * sizeof(Record);
        platfrom::persistent::write(address, buffer, sizeof(flood));
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
            uint16_t i;
            i = j*sizeof(Record);
            byte* b = platfrom::persistent::read(i);
            Record record;
            memcpy(&record, b, sizeof(Record));
            record.print();
        }
        Serial.println("--------------------End journal----------------------");
    }
}

void Wallet::generatePrivateKey() {
    HDPrivateKey hd("ABCDEFG09");
    byte seed[64] = { 0 }; 
    hd.setSecret((uint8_t*) seed); 

    uint16_t pointer = (journalSize + 1) * sizeof(Record);
    platfrom::persistent::write(pointer, (byte *)hd, masterKeyLength);
    Serial.println("Master key has been written!");
}

void Wallet::printPublicKey() {
    uint16_t pointer = (journalSize + 1) * sizeof(Record);
    byte * b = platfrom::persistent::read(pointer);
    char hd[masterKeyLength];
    memcpy(&hd, b, masterKeyLength);
    Serial.println("This is the master key:");
    Serial.println(hd);

    PublicKey pubKey = hd.publicKey()

    Serial.println("This is public key key:");
    Serial.println(pubKey);
}

void Wallet::signTransaction() {
    uint16_t pointer = (journalSize + 1) * sizeof(Record);
    byte * b = platfrom::persistent::read(pointer);
    char PrivateKey hd[masterKeyLength];
    memcpy(&hd, b, masterKeyLength);
    Serial.println("This is the master key:");
    Serial.println(hd);

    Signature signature = hd.sign(hash);
    Serial.println(signature);
}
