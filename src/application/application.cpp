#include "crypto.h"
#include "journal.h"
#include "./../platform/platform.h"

/*
enum apiCommand {
  create_master_key,
  get_list_of_child_pubkeys,
  sign_transaction,
  get_journal
};
*/

/*
HDPrivateKey hd;
HDPrivateKey hd2;
HDPublicKey hd_pub;
PrivateKey child_private_key;
PublicKey child_public_key;
Signature signature;
char mySeed[64] = { 0 }; 
char* myKey = "test sfgadfhagsgh" ;
String message = "Hello, Roman!";
byte hash[64] = { 0 }; // hash
uint32_t event_number = 0;  //value that shows where is the last record in storage
bool storage_is_full = false; //if this variable is false you can make operations, if true - you can not
uint16_t long_numer = 0;      //256 - digital number to generate private master key "010011101010..."
*/

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

    switch(oper) {
        case 0:
        // genRandomNumber(mySeed, 64);
        // Serial.println("Random number2: " + toHex(mySeed, 64));
        break;
        
        /*
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
        writeRecord( event,  event_number);
        break;
        
        case 8:
        printJournal();
        break;  
        
        case 9:
        cleanJournal();
        break;
            
        break;
        */
    }
}