#pragma once
#include <string>

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
        std::string readString();
        int log(std::string);
    }

    namespace persistent {
        int read(__UINT32_TYPE__ addr, __UINT8_TYPE__ *dst, __UINT32_TYPE__ len);
        int write(__UINT32_TYPE__ addr, __UINT8_TYPE__ *src, __UINT32_TYPE__ len);
    }
}
