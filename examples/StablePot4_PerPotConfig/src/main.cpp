// examples/StablePot4_PerPotConfig/src/main.cpp
#include <StablePot.h>

StablePot knob1(15, StablePot::STABLEPOT4);
StablePot knob2(32, StablePot::STABLEPOT4);

void setup() {
  Serial.begin(115200);
  
  // Customize individually
  knob1.configure(0.3f, 0.75f, 0.002f, 0.0007f, 150, 4);
  knob2.configure(0.2f, 0.8f, 0.001f, 0.0005f, 100, 5);
}

void loop() {
  knob1.update();
  knob2.update();
  
  Serial.print(knob1.getProcessedADC());
  Serial.print('\t');
  Serial.println(knob2.getProcessedADC());
  
  delay(1);
}