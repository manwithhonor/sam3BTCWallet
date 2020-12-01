#include <Arduino.h>
#include "platform.h"
#include "application.h"
#include "Bitcoin.h"
#include "ecdsa.h"
#include "DueFlashStorage.h"

void setup() {
    Serial.begin(115200);
    // init_clocks();
}

void loop() {
    if (Serial.available() > 0) {
        // loop_iteration();
    }    
}
