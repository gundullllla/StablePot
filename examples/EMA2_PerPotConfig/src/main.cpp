// examples/EMA2_PerPotConfig/src/main.cpp
#include <StablePot.h>

StablePot knob1(15, StablePot::EMA2);
StablePot knob2(32, StablePot::EMA2);
// Note: The pins 15 and 32 are examples; you can change them to any valid GPIO pin on your ESP32 board.

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  // Set the ADC resolution and attenuation for better range and accuracy.
  // The ADC resolution is set to 12 bits (0-4095) and attenuation to 11dB for a wider input range.
  // This allows the ADC to read voltages up to approximately 3.6V.
  // The attenuation setting is important for the ESP32 to read higher voltages correctly.

  
  // Initialize the StablePot instances with different configurations.
  knob1.setAlphas(0.15f, 0.6f); //alpha1 for primary smoothing, alpha2 for secondary smoothing 
  knob1.setRounding(6);
  
  knob2.setAlphas(0.25f, 0.8f);
  knob2.setRounding(3);
}

void loop() {
  knob1.update();
  knob2.update();
  
  Serial.print(knob1.getProcessedValue(), 3); //rounding set to 3
  Serial.print('\t');
  Serial.println(knob2.getProcessedValue(), 6); //rounding set to 6
  // Note: getProcessedValue() returns a float (0-1), getProcessedADC() returns an int (0-4095 for 12-bit ADC)
  delay(1);
}
