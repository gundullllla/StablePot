#include <StablePot.h>

// Global configuration for all StablePot3 instances
const float GLOBAL_ALPHA_P = 0.15f;        // Primary smoothing
const float GLOBAL_ALPHA_S = 0.4f;       // Secondary smoothing
const float GLOBAL_THRESH_S = 0.0009f;     // Small threshold
const float GLOBAL_THRESH_B = 0.0016f;     // Big threshold
const uint16_t GLOBAL_FILTER_TIME = 111;   // Filter window (ms)
const uint8_t GLOBAL_ROUNDING = 6;         // Rounding precision

// Create multiple pots with StablePot3 algorithm
// NOTE: Changed "tone" to "toneCtrl" to avoid conflict with Arduino tone() function
StablePot volume(15, StablePot::STABLEPOT3);
StablePot balance(32, StablePot::STABLEPOT3);
StablePot toneCtrl(33, StablePot::STABLEPOT3);  

void setup() {
  Serial.begin(115200);
  
  // Configure ESP32 ADC settings (essential)
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  
  // Apply global configuration to all instances
  volume.configure(
    GLOBAL_ALPHA_P,
    GLOBAL_ALPHA_S,
    GLOBAL_THRESH_S,
    GLOBAL_THRESH_B,
    GLOBAL_FILTER_TIME,
    GLOBAL_ROUNDING
  );
  
  balance.configure(
    GLOBAL_ALPHA_P,
    GLOBAL_ALPHA_S,
    GLOBAL_THRESH_S,
    GLOBAL_THRESH_B,
    GLOBAL_FILTER_TIME,
    GLOBAL_ROUNDING
  );
  
  toneCtrl.configure(  
    GLOBAL_ALPHA_P,
    GLOBAL_ALPHA_S,
    GLOBAL_THRESH_S,
    GLOBAL_THRESH_B,
    GLOBAL_FILTER_TIME,
    GLOBAL_ROUNDING
  );
  
  Serial.println("StablePot3 Global Configuration Example");
  Serial.println("All pots share the same smoothing parameters");
  Serial.println("Volume | Balance | Tone");
}

void loop() {
  // Update all potentiometers
  volume.update();
  balance.update();
  toneCtrl.update();  
  
  // Get processed values
  const int volValue = volume.getProcessedADC();
  const int balValue = balance.getProcessedADC(); 
  const int toneValue = toneCtrl.getProcessedADC();   
  // Print formatted output
  Serial.print(volValue);
  Serial.print(" | ");
  Serial.print(balValue);
  Serial.print(" | ");
  Serial.println(toneValue);
  
  delay(10);  // 100Hz refresh rate
}
