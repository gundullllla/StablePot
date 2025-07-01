// examples/StablePot4_PerPotConfig/src/main.cpp
#include <StablePot.h>

StablePot knob1(15, StablePot::STABLEPOT4);
StablePot knob2(32, StablePot::STABLEPOT4);

void setup() {
  Serial.begin(115200);
  
  // Customize individually - knob1 is set for maximizing stability, knob2 is set for quick response.
  //alpha1,alpha2,threshold1,threshold2,filter_time_ms,rounding_strength
  knob1.configure(0.2f, 0.5f, 0.0004f, 0.0029f, 122, 6); 
  knob2.configure(0.2f, 0.8f, 0.001f, 0.0005f, 22, 6);
}

void loop() {
  knob1.update();
  knob2.update();
  
  Serial.print(knob1.getProcessedADC());
  Serial.print('\t');
  Serial.println(knob2.getProcessedADC());
  
  delay(1);
}
