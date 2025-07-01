#include "EMAFilter.h"
#include <Arduino.h>

EMAFilter::EMAFilter(float alpha_p, float alpha_s)
  : _alpha_p(constrain(alpha_p, 0.0001f, 1.0f)),
    _alpha_s(constrain(alpha_s, 0.0001f, 1.0f)) {}

EMAFilter::EMAFilter(const Config& config)
  : _alpha_p(constrain(config.alpha_p, 0.0001f, 1.0f)),
    _alpha_s(constrain(config.alpha_s, 0.0001f, 1.0f)),
    _thresh_s(fmaxf(config.thresh_s, 0.0001f)),
    _thresh_b(fmaxf(config.thresh_b, _thresh_s + 0.0001f)),
    _filter_window(config.filter_time) {}

void EMAFilter::update(uint16_t raw) {
  const float value = raw / 4095.0f;
  
  if (_first) {
    _smoothed = _secondary = _stored = value;
    _first = false;
    return;
  }
  
  // Primary EMA
  _smoothed += _alpha_p * (value - _smoothed);
  
  // State machine
  const uint32_t now = millis();
  switch (_state) {
    case STABLE:
      if (fabs(_smoothed - _stored) > _thresh_b) {
        _state = TRANSITION;
        _last_change = now;
        _stored = _smoothed;
      }
      break;
      
    case TRANSITION:
      if ((now - _last_change) > _filter_window) {
        _state = (fabs(_smoothed - _stored) > _thresh_s)
                 ? TRANSITION : STABLE;
        _stored = _smoothed;
      }
      break;
  }
  
  // Secondary EMA
  _secondary += _alpha_s * (_stored - _secondary);
}

void EMAFilter::reconfigure(const Config& config) {
  _alpha_p = constrain(config.alpha_p, 0.0001f, 1.0f);
  _alpha_s = constrain(config.alpha_s, 0.0001f, 1.0f);
  _thresh_s = fmaxf(config.thresh_s, 0.0001f);
  _thresh_b = fmaxf(config.thresh_b, _thresh_s + 0.0001f);
  _filter_window = config.filter_time;
}

void EMAFilter::reconfigure(float alpha_p, float alpha_s, 
                           float thresh_s, float thresh_b,
                           uint16_t filter_time) {
  _alpha_p = constrain(alpha_p, 0.0001f, 1.0f);
  _alpha_s = constrain(alpha_s, 0.0001f, 1.0f);
  _thresh_s = fmaxf(thresh_s, 0.0001f);
  _thresh_b = fmaxf(thresh_b, _thresh_s + 0.0001f);
  _filter_window = filter_time;
}

void EMAFilter::setAlphas(float primary, float secondary) {
  _alpha_p = constrain(primary, 0.0001f, 1.0f);
  _alpha_s = constrain(secondary, 0.0001f, 1.0f);
}

void EMAFilter::setThresholds(float small, float big) {
  _thresh_s = fmaxf(small, 0.0001f);
  _thresh_b = fmaxf(big, _thresh_s + 0.0001f);
}

void EMAFilter::setFilterTime(uint16_t time_ms) {
  _filter_window = time_ms;
}