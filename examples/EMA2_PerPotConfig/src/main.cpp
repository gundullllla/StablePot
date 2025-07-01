// examples/EMA2_PerPotConfig/src/main.cpp
#include <StablePot.h>

StablePot knob1(15, StablePot::EMA2);
StablePot knob2(32, StablePot::EMA2);

void setup() {
  Serial.begin(115200);
  
  knob1.setAlphas(0.15f, 0.6f);
  knob1.setRounding(4);
  
  knob2.setAlphas(0.25f, 0.8f);
  knob2.setRounding(3);
}

void loop() {
  knob1.update();
  knob2.update();
  
  Serial.print(knob1.getProcessedValue(), 4);
  Serial.print('\t');
  Serial.println(knob2.getProcessedValue(), 4);
  
  delay(1);
}