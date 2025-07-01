// examples/StablePot1_Basic/src/main.cpp
#include <StablePot.h>

StablePot knob(15, StablePot::STABLEPOT1);

void setup() {
  Serial.begin(115200);
}

void loop() {
  knob.update();
  
  Serial.print(knob.getRawValue(), 4);
  Serial.print('\t');
  Serial.print(knob.getSmoothedValue(), 4);
  Serial.print('\t');
  Serial.println(knob.getProcessedValue(), 4);
  
  delay(1);
}