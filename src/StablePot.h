#pragma once
#include <Arduino.h>
#include "EMAFilter.h"

class StablePot {
public:
  enum Algorithm {
    STABLEPOT1,
    STABLEPOT2,
    STABLEPOT3,
    STABLEPOT4,
    EMA1,
    EMA2
  };

  StablePot(uint8_t pin, Algorithm algo);
  ~StablePot();
  
  void update();
  float getRawValue() const;
  float getSmoothedValue() const;
  float getProcessedValue() const;
  int getProcessedADC() const;
  
  // Configuration methods
  void configure(float alpha_p, float alpha_s, 
                float thresh_s, float thresh_b,
                uint16_t filter_time, uint8_t rounding);
  void setAlphas(float primary, float secondary);
  void setThresholds(float small, float big);
  void setFilterTime(uint16_t time_ms);
  void setRounding(uint8_t strength);

private:
  const uint8_t _pin;
  EMAFilter* _filter;
  Algorithm _algo;
  uint8_t _rounding_strength = 6;
  float _rounding_factor = 1e6;
  float _last_raw = 0;

  void applyPreset();
};