#pragma once
#include <cstdint>
#include <cmath>

class EMAFilter {
public:
  struct Config {
    float alpha_p;
    float alpha_s;
    float thresh_s;
    float thresh_b;
    uint16_t filter_time;
  };
  
  EMAFilter(float alpha_p, float alpha_s);
  EMAFilter(const Config& config);
  
  void update(uint16_t raw_value);
  void reconfigure(const Config& config);
  void reconfigure(float alpha_p, float alpha_s, 
                  float thresh_s, float thresh_b,
                  uint16_t filter_time);
  void setAlphas(float primary, float secondary);
  void setThresholds(float small, float big);
  void setFilterTime(uint16_t time_ms);
  
  float getSmoothed() const { return _smoothed; }
  float getProcessed() const { return _secondary; }

private:
  enum State : uint8_t { STABLE, TRANSITION };
  
  float _smoothed = 0;
  float _secondary = 0;
  float _stored = 0;
  float _alpha_p, _alpha_s;
  float _thresh_s = 0.0030f;
  float _thresh_b = 0.0050f;
  uint32_t _last_change = 0;
  uint16_t _filter_window = 50;
  State _state = STABLE;
  bool _first = true;
};