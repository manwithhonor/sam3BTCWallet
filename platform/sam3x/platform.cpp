#include "DueFlashStorage.h"
#include "rtc_clock.h"

#include "./../../src/platform/platform.h"

RTC_clock rtc_clock(RC); // Select the Slowclock source
DueFlashStorage dueFlashStorage;

namespace platform {
    namespace clocks {
        void init() {
            rtc_clock.init();               //init real time clock to write proper time while recording events
            rtc_clock.set_time(__TIME__);   //__TIME__ and __DATA__ are taken from timestamp when this code was compiled
            rtc_clock.set_date(__DATE__);   //if __DATA__ is 1.1.2007 then this is a problem of compiler  
        }

        platfrom::clocks::Time get() {
            platfrom::clocks::Time time;
    
            time.seconds = rtc_clock.get_seconds();
            time.minutes = rtc_clock.get_minutes();
            time.hours   = rtc_clock.get_hours();
            time.day     = rtc_clock.get_days();
            time.month   = rtc_clock.get_months();
            time.year    = rtc_clock.get_years();
            
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
        byte* read(uint32_t address) {
            return dueFlashStorage.readAddress(address);
        }

        int write(uint32_t address, byte *data, uint32_t dataLength) {
            dueFlashStorage.write(address, data, dataLength);
            return 0 ;
        }
    }
}
