#include "crypto.h"
#include "journal.h"
#include "application.h"
#include "Bitcoin.h"
#include "Hash.h"
#include "./../platform/platform.h"


enum apiCommand {
  create_master_key,
  get_list_of_child_pubkeys,
  sign_transaction,
  get_journal
};

byte  mySeed[64] = { 0 }; 
char* myKey = "test sfgadfhagsgh" ;
String message = "Hello, Roman!";
byte hash[64] = { 0 }; // hash
HDPrivateKey hd;
HDPrivateKey hd2;
HDPublicKey hd_pub;
PrivateKey child_private_key;
PublicKey child_public_key;
Signature signature;

// write event_number to flash memory
void writeEventNumber(){

}

// read event_number from flash memory
void readEventNumber(){

}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}



/*
std::pair<int, std::string> parseCommand(std::string cmd) {
  auto tokens = std::strtok(cmd.c_str(), "_")
  return
}
*/

int loopIteration() {
    /*
    std::string rawCmd  = platfrom::console::readString();
    auto cmd = parseCommand(cmd);
    int oper = cmd.first;
    int user = cmd.second;
    */

    int oper = 0;
    // std::string user = "Roman";
    String stringVar  = Serial.readString();
    String comand = getValue(stringVar, '_', 0);
    String user = getValue(stringVar, '_', 1);

    Record event = createRecord (user, "Test", "Success"  );
    //print_record( event);

    
    int intVar = comand.toInt();
    //int inByte = Serial.read()-'0';
    switch(intVar) {
        case 0:
        oper= crypto::generate::randomInt(mySeed);
        Serial.println("Random number2: " + toHex(mySeed, 64));
        break;
        
        
        case 1:
        hd = genMasterPrivateKey(mySeed);
        Serial.print("Master private key: ");
        Serial.println(hd);
        
        hd_pub = getMasterPublicKey(hd);
        Serial.print("Master public key: ");
        Serial.println(hd_pub);
        break;
        
        case 2:
        child_private_key = getChildPrivateKey(hd, 2);
        Serial.print("Child private key: ");
        Serial.println(child_private_key);

        child_public_key = getChildPublicKey(child_private_key);
        Serial.print("Child public key: ");
        Serial.println(child_public_key);
        break;
        
        case 3:     
        getHash(message, hash, 64);
        Serial.println("Test sha: " + toHex(hash, 64));
        
        signature = signTransaction(child_private_key, hash);
        Serial.print("Signature: ");
        Serial.println(signature);
        break;

        case 4:
        write_master_key(myKey);
        break;

        case 5:
        write_master_key2(mySeed);
        break;

        case 6:
        print_master_key2();
        break;
        
        
        case 7:
        event = createRecord (user, "Test", "Success"  );
        printRecord( event);
        // writeRecord( event,  event_number);
        writeRecord( event,  0);
        break;
        
        case 8:
        printJournal();
        break;  
        
        case 9:
        cleanJournal();
        break;
            
        break;
        
    }
}