#pragma once
#include <string>
#include "./../bitcoin/Bitcoin.h"


namespace platfrom {
    class string {
        char* c_str();
    };

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

        int init();
        Time get();
    }

    namespace console {
        //std::string readString();
        String readString();
        int printString(String message);
        // int log(std::string);
    }

    namespace persistent {
        byte* read(uint32_t address);
        int write(uint32_t address, byte *data, uint32_t dataLength) ;
    }
}
