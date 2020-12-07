#include <Arduino.h>
#include "platform.h"
#include "application.h"

void setup() {
    Serial.begin(115200);
    platform::clocks::init();
}

  
// the loop function runs over and over again forever
void loop() {
    if (Serial.available() > 0) {
        loopIteration();
    }
}