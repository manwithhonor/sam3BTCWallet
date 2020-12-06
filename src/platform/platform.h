#pragma once
#include <stdint.h>
#include <Arduino.h>

namespace platform {
    namespace clocks {
        struct Time {
            uint8_t seconds;
            uint8_t minutes;
            uint8_t hours;
            uint8_t padding;
            uint16_t year;
            uint8_t month;
            uint8_t day;
        };

        void init();
        Time get();
    }

    namespace console {
        String readString();
        int print(String message);
        int printString(String message);
    }

    namespace persistent {
        int read(uint32_t address, byte* seed);
        int write(uint32_t address, byte *data, uint32_t dataLength) ;
    }
}
