#include <Arduino.h>
#include "platform.h"
#include "application.h"
#include "Bitcoin.h"
#include "ecdsa.h"
#include "DueFlashStorage.h"
#include "rtc_clock.h"

void setup() {
    Serial.begin(115200);

}

  
// the loop function runs over and over again forever
void loop() {
    if (Serial.available() > 0) {
        loopIteration();
    }
}