#include "./../../src/platform/platform.h"
#include "DueFlashStorage.h"
#include "rtc_clock.h"

RTC_clock rtc_clock(RC); // Select the Slowclock source
// DueFlashStorage dueFlashStorage;

namespace platfrom {
    namespace clocks {
        int init() {
            
            rtc_clock.init();               //init real time clock to write proper time while recording events
            rtc_clock.set_time(__TIME__);   //__TIME__ and __DATA__ are taken from timestamp when this code was compiled
            rtc_clock.set_date(__DATE__);   //if __DATA__ is 1.1.2007 then this is a problem of compiler  
            return 0;
        }
        
        Time get() {
            platfrom::clocks::Time time;
            
            time.seconds = rtc_clock.get_seconds();
            time.minutes = rtc_clock.get_minutes();
            time.hours = rtc_clock.get_hours();
            time.day = rtc_clock.get_days();
            time.month = rtc_clock.get_months();
            time.year = rtc_clock.get_years();
            
            return time;
        }
    }

    namespace console {
        std::string readString() {

        }

        int log(std::string) {

        }
    }

    namespace persistent {
        int read(__UINT32_TYPE__ addr, __UINT8_TYPE__ *dst, __UINT32_TYPE__ len) {

        }

        int write(__UINT32_TYPE__ addr, __UINT8_TYPE__ *src, __UINT32_TYPE__ len) {

        }
    }
}
