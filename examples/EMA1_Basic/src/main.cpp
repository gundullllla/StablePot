#include <StablePot.h>

// Using EMA1 algorithm with its default preset
StablePot knob(15, StablePot::EMA1);

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
