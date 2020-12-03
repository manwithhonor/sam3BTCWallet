#include "journal.h"
#include "./../../src/platform/platform.h"
#include "./../bitcoin/Bitcoin.h"
#include "application.h"


uint32_t event_number = 0;  //value that shows where is the last record in storage
bool storage_is_full = false; //if this variable is false you can make operations, if true - you can not

Record createRecord(String user, const char* operation, const char* status) {
    Record event;
    event.time = platfrom::clocks::get();
    //event.user = user.c_str();
    user.toCharArray(event.user, 16);
    event.operation = (char*) operation;
    event.status = (char*) status;
    return event;
}

void printRecord(Record record) {
    Serial.print("Time: ");
    Serial.print(record.time.hours);
    Serial.print(":");
    Serial.print(record.time.minutes);
    Serial.print(":");
    Serial.print(record.time.seconds);
    Serial.print(" ");
    Serial.print(record.time.day);
    Serial.print(".");
    Serial.print(record.time.month);
    Serial.print(".");
    Serial.print(record.time.year);
    Serial.print(", User: ");
    Serial.print(record.user);
    Serial.print(", Operation: ");
    Serial.print(record.operation);
    Serial.print(", Status: ");
    Serial.println(record.status);
}

void writeRecord(Record record, uint32_t log_number) {
    if (log_number < MAXIMUM_SIZE) {
        uint32_t address = log_number * sizeof(Record);
        byte buffer[sizeof(record)];
        memcpy(buffer, &record, sizeof(record));
        // dueFlashStorage.write(address, buffer, sizeof(record));
        platfrom::persistent::write(address, buffer, sizeof(record));
        event_number++;
        Serial.println("A record was written!");
    }
    else  {
        Serial.println("The storage is full!");
        storage_is_full = true;
    }
}

void printJournal() {
    if (event_number == 0)
        Serial.println("Storage is empty!");
    else {
        Serial.println("--------------------Begin storage--------------------");
        for (uint32_t j = 0; j < event_number; j++) {
            uint16_t i;
            i = j*sizeof(Record);
            byte * b = platfrom::persistent::read(i);
            // byte * b = dueFlashStorage.readAddress(i);
            Record temp_record;
            memcpy(&temp_record, b, sizeof(Record));
            printRecord(temp_record);
        }
        Serial.println("--------------------End storage----------------------");
    }
}

void cleanJournal(void) {
  Record flood = {{0, 0, 0, 0, 0, 0}, "", "", ""};
  byte buffer[sizeof(flood)];
  memcpy(buffer, &flood, sizeof(flood));
  uint32_t address;
   
  for (uint32_t i = 0; i < event_number; i++)
      // writeRecord(flood, i);
      address = i * sizeof(Record);
      // dueFlashStorage.write(address, buffer, sizeof(flood));
      platfrom::persistent::write(address, buffer, sizeof(flood));
      
  Serial.println("Log was cleaned!");
  event_number = 0;
  storage_is_full = false;
}
