#include "application.h"
#include "crypto.h"
#include "wallet.h"

#include "Bitcoin.h"
#include "Hash.h"

#include "./../platform/platform.h"

Wallet wallet;

byte hash[64] = { 0 }; 
byte seed[64] = { 0 }; 

enum commands {
    generateNewPrivateKey = 0,
    printPublicKey        = 1,
    signTransaction       = 2,
    printJournal          = 3
};

String getValue(String data, char separator, int index) {
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length()-1;

    for (int i=0; i<=maxIndex && found<=index; i++) {
        if (data.charAt(i)==separator || i==maxIndex) {
            found++;
            strIndex[0] = strIndex[1]+1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }

    return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

int loopIteration() {
    int oper = 0;
    String rawCmd  = platform::console::readString();
    String cmd = getValue(rawCmd, '_', 0);
    String user = getValue(rawCmd, '_', 1);
    commands intCmd = (commands) cmd.toInt();

    switch(intCmd) {
    case generateNewPrivateKey:
        wallet.generateSeed();
        break;
        
    case printPublicKey:
        wallet.readSeed(seed);
        //wallet.printPublicKey();
        break;
        
    case signTransaction:
        wallet.signTransaction(hash);
        break;

    case printJournal:
       // wallet.printJournal();
        break;  

    default: 
        break;
        
    }
}
