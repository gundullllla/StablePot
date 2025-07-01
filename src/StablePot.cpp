#include "StablePot.h"

namespace Presets {
  const EMAFilter::Config StablePot1 = {0.25f, 0.7f, 0.0018f, 0.0006f, 122};
  const EMAFilter::Config StablePot2 = {0.25f, 0.7f, 0.0008f, 0.0004f, 122};
  const EMAFilter::Config EMA1 = {0.25f, 0.7f, 0.0030f, 0.0050f, 50};
}

StablePot::StablePot(uint8_t pin, Algorithm algo)
  : _pin(pin), _algo(algo) {
  switch(algo) {
    case STABLEPOT1: _filter = new EMAFilter(Presets::StablePot1); break;
    case STABLEPOT2: _filter = new EMAFilter(Presets::StablePot2); break;
    case EMA1: _filter = new EMAFilter(Presets::EMA1); break;
    default: _filter = new EMAFilter(0.25f, 0.7f);
  }
  applyPreset();
}

StablePot::~StablePot() { 
  delete _filter; 
}

void StablePot::update() {
  const int raw = analogRead(_pin);
  _last_raw = raw / 4095.0f;
  _filter->update(raw);
}

float StablePot::getRawValue() const { 
  return _last_raw; 
}

float StablePot::getSmoothedValue() const { 
  return _filter->getSmoothed(); 
}

float StablePot::getProcessedValue() const {
  const float value = _filter->getProcessed();
  return round(value * _rounding_factor) / _rounding_factor;
}

int StablePot::getProcessedADC() const {
  return static_cast<int>(getProcessedValue() * 4095.0f);
}

void StablePot::applyPreset() {
  switch(_algo) {
    case STABLEPOT1:
      _filter->reconfigure(Presets::StablePot1);
      _rounding_strength = 6;
      break;
    case STABLEPOT2:
      _filter->reconfigure(Presets::StablePot2);
      _rounding_strength = 6;
      break;
    case EMA1:
      _filter->reconfigure(Presets::EMA1);
      _rounding_strength = 6;
      break;
    default:
      _rounding_strength = 6;
      break;
  }
  _rounding_factor = powf(10.0f, _rounding_strength);
}

void StablePot::configure(float alpha_p, float alpha_s, 
                         float thresh_s, float thresh_b,
                         uint16_t filter_time, uint8_t rounding) {
  _filter->reconfigure(alpha_p, alpha_s, thresh_s, thresh_b, filter_time);
  _rounding_strength = constrain(rounding, 0, 6);
  _rounding_factor = powf(10.0f, _rounding_strength);
}

void StablePot::setAlphas(float primary, float secondary) {
  _filter->setAlphas(primary, secondary);
}

void StablePot::setThresholds(float small, float big) {
  _filter->setThresholds(small, big);
}

void StablePot::setFilterTime(uint16_t time_ms) {
  _filter->setFilterTime(time_ms);
}

void StablePot::setRounding(uint8_t strength) {
  _rounding_strength = constrain(strength, 0, 6);
  _rounding_factor = powf(10.0f, _rounding_strength);
}