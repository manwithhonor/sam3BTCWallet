#include "./../../src/platform/platform.h"

#include <DueFlashStorage.h>
#include <RTCDue.h>

RTCDue rtc(XTAL);
DueFlashStorage dueFlashStorage;

namespace platform {
    namespace clocks {
        void init() {
            rtc.begin();
        }

        platform::clocks::Time get() {
            platform::clocks::Time time;
    
            time.seconds = rtc.getSeconds();
            time.minutes = rtc.getMinutes();
            time.hours   = rtc.getHours();
            time.day     = rtc.getDay();
            time.month   = rtc.getMonth();
            time.year    = rtc.getYear();
            
            return time;
        }
    }

    namespace console {
        String readString() {
            String input_string = Serial.readString();
            return input_string;
        }

        int print(String message) {
            Serial.print(message);
            return 0;
        }

        int printString(String message) {
            Serial.println(message);
            return 0;
        }
    }

    namespace persistent {
        int read(uint32_t address, byte* seed, uint32_t dataLength) {
            byte * b = dueFlashStorage.readAddress(address);
            memcpy(seed, b, dataLength);
            return 0;
        }

        int write(uint32_t address, byte *data, uint32_t dataLength) {
            dueFlashStorage.write(address, data, dataLength);
            return 0;
        }
    }
}
