#include "application.h"
#include "crypto.h"
#include "wallet.h"

#include "Bitcoin.h"
#include "Hash.h"
#include "ArduinoJson.h"

#include "./../platform/platform.h"
#include <Regexp.h>

Wallet wallet;

enum commands {
    generateNewSeed = 0,
    printPublicKeys = 1,
    signTransaction = 2,
    printJournal    = 3,
    cleanJournal    = 4,
    deleteSeed      = 5,
    logOverload     = 6
};

int checkLog(uint8_t journalTail){
    DynamicJsonDocument jsonOutput(1024);
    
    if (journalTail >= 255){
        // Serial.println("Operation is forbidden, because log is full. Please clear it.");
        jsonOutput["Error message"] = "Operation is forbidden, because log is full. Please clear it.";
        serializeJson(jsonOutput, Serial);
        return 1;
    }
    return 0;
}

int checkSeed(uint8_t seedFlag, int oper){
    Record event;
    DynamicJsonDocument jsonOutput(1024);
    if (seedFlag == 0) {
        // Serial.println("ERROR: master seed is not found");
        jsonOutput["Error message"] = "ERROR: master seed is not found";
        serializeJson(jsonOutput, Serial);

        if (oper == 0) {
            event = Record("Roman", "printPublicKeys", "Failed");            
        }
        else {
            event = Record("Roman", "signTransaction", "Failed");
        }

        wallet.appendJournalRecord(event);

        return 1;
    }
    return 0;
}

int loopIteration() {
    String rawCmd  = platform::console::readString();
    DynamicJsonDocument jsonInput(1024);
    DynamicJsonDocument jsonOutput(1024);
    jsonOutput["Error message"] = "";
    
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

    MatchState ms;
    char charBuf[50];
    char matchResult;

    
    switch(intCmd) {
    case generateNewSeed:
        if (checkLog(wallet.journalTail)) return 1;
        wallet.generateSeed();
        event = Record(user, "generateNewSeed", "Success");
        wallet.appendJournalRecord(event);
        break;
        
    case printPublicKeys:
        if (checkLog(wallet.journalTail)) return 1;
        if (checkSeed(wallet.seedFlag, 0)) return 1;

        hash_str.toCharArray(charBuf, 50);
        ms.Target (charBuf);
        matchResult = ms.Match ("^[1-9]\d*$");
        if (matchResult > 0){
            keyAmount =  hash_str.toInt();
            keyType =  derivationPath.toInt();
            wallet.printPublicKeys(keyAmount, keyType);
        }
        else {
            // Serial.println("ERROR: wrong key amount");
            jsonOutput["Error message"] = "ERROR: wrong key amount";
            serializeJson(jsonOutput, Serial);

            event = Record(user, "printPublicKeys", "Failed");
            wallet.appendJournalRecord(event);
            break;
        }

        event = Record(user, "printPublicKeys", "Success");
        wallet.appendJournalRecord(event);        
        break;
        
    case signTransaction:
        if (checkLog(wallet.journalTail)) return 1;
        if (checkSeed(wallet.seedFlag, 1)) return 1;

        derivationPath.toCharArray(charBuf, 50);
        ms.Target (charBuf);
        matchResult = ms.Match ("[mM]/[0-1]/[0-9]+");
        if (matchResult > 0){
            hash_str.toCharArray(hash, 64);
            wallet.signTransaction( (byte*) hash, derivationPath);
        }
        else {
            // Serial.println("ERROR: wrong derivation path");
            jsonOutput["Error message"] = "ERROR: wrong derivation path";
            serializeJson(jsonOutput, Serial);

            event = Record(user, "signTransaction", "Failed");
            wallet.appendJournalRecord(event);
            break;
        }

        event = Record(user, "signTransaction", "Success");
        wallet.appendJournalRecord(event);
        break;

    case printJournal:
        wallet.printJournal();
        event = Record(user, "printJournal", "Success");
        wallet.appendJournalRecord(event);
        break;  

    case cleanJournal:
        wallet.cleanJournal();
        break;  

    // This is for test purpose only
    /*case deleteSeed:
        wallet.seedFlag = 0;
        wallet.writeSeedFlag(0);
        Serial.println("Seed was deleted!");
        break;  
    
    case logOverload:
        wallet.journalTail = 255;
        wallet.writeJoutnalTail(255);

        if (checkLog(wallet.journalTail)) return 1;

        break;*/
    default: 
        break;
        
    }
}
