#include "./../../src/platform/platform.h"
#include "./../platform/platform.h"
#include "./../bitcoin/Bitcoin.h"

struct Record{
  platfrom::clocks::Time  time;
  char user[16];
  char* operation;
  char* status;
};

Record createRecord(String user, const char* operation, const char* status);

void printRecord(Record record);
void writeRecord(Record record, uint32_t log_number);

void printJournal();
void cleanJournal(void);
