#include "application.h"
#include "crypto.h"
#include "wallet.h"

#include "Bitcoin.h"
#include "Hash.h"
#include "ArduinoJson.h"

#include "./../platform/platform.h"

Wallet wallet;

enum commands {
    generateNewSeed = 0,
    printPublicKeys = 1,
    signTransaction = 2,
    printJournal    = 3
};

int loopIteration() {
    String rawCmd  = platform::console::readString();
    DynamicJsonDocument jsonInput(1024);
    deserializeJson(jsonInput, rawCmd);

    String cmd = jsonInput["command"];
    commands intCmd = (commands) cmd.toInt();
    
    // this is for test purpose only
    Record event;
    //int keyAmount = jsonInput["data"][0];
    //int keyType = jsonInput["data"][1];
    int keyAmount;
    int keyType;
    String hash_str = jsonInput["data"][0];
    String derivationPath = jsonInput["data"][1];
    String buffer;
    char user[16] =  "Roman";
    char operation[32] = "Test";
    char status[16] = "Success";
    char hash[64] = { 0 };


    /*if (cmd.toInt() == 1) {
        keyAmount = jsonInput["data"]["keyAmount"];
        keyType = jsonInput["data"]["keyType"];
    } else if (cmd.toInt() == 2) {
        hash_str  = jsonInput["data"]["hash"];
        derivationPath = jsonInput["data"]["derivationPath"];        
        hash_str.toCharArray(hash, 64);
    }*/
    
    switch(intCmd) {
    case generateNewSeed:
        wallet.generateSeed();

        buffer = "generateNewSeed";
        buffer.toCharArray(operation, 32);
        event = Record(user, operation, status);
        wallet.appendJournalRecord(event);
        break;
        
    case printPublicKeys:
        // fix later
        keyAmount =  hash_str.toInt();
        keyType =  derivationPath.toInt();
        wallet.printPublicKeys(keyAmount, keyType);

        buffer = "printPublicKeys";
        buffer.toCharArray(operation, 32);
        event = Record(user, operation, status);
        wallet.appendJournalRecord(event);
        break;
        
    case signTransaction:
        hash_str.toCharArray(hash, 64);
        wallet.signTransaction( (byte*) hash, derivationPath);

        buffer = "signTransaction";
        buffer.toCharArray(operation, 32);
        event = Record(user, operation, status);
        wallet.appendJournalRecord(event);
        break;

    case printJournal:
        buffer = "printJournal";
        buffer.toCharArray(operation, 32);
        event = Record(user, operation, status);
        wallet.appendJournalRecord(event);

        wallet.printJournal();
        // wallet.cleanJournal();
        break;  

    default: 
        break;
        
    }
}
