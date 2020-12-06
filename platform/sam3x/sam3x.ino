#include <Arduino.h>
#include "platform.h"
#include "application.h"
#include "Bitcoin.h"
#include "ecdsa.h"
#include "DueFlashStorage.h"
#include "rtc_clock.h"

#ifndef ARDUINO
#define RAW
#else
#include <Arduino.h>
#endif

/* settings for Arduino */
#ifdef ARDUINO
#define USE_ARDUINO_STRING 1 /* Arduino String implementation (WString.h) */
#define USE_STD_STRING     0 /* Standard library std::string */
#endif

#ifdef RAW
#define USE_ARDUINO_STRING 0 /* Arduino String implementation (WString.h) */
#define USE_STD_STRING     1 /* Standard library std::string */
#endif

void setup() {
    Serial.begin(115200);
    platfrom::clocks::init();
}

  
// the loop function runs over and over again forever
void loop() {
    if (Serial.available() > 0) {
        loopIteration();
    }
}