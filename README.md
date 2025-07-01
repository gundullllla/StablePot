# StablePot Library - Potentiometer Smoothing for ESP32
### 6 Distinct Smoothing Algorithms optimized for ESP32 ADCs ###

_Disclaimer: i don't know much about coding and made this library including documentation with the use of AI._

### Circuit Design

connect wiper of B10k Potentiometer to ADC Pin and 0.1μF Cap to wiper on one side and GND on other side.
```         
3.3V ────┐
         │
         │
         │
B10K Pot ║║──┬─ ESP32 ADC Pin
         │   │ 
         │  0.1μF Cap
         │   │
 GND ────┘───┘
```

## Installation
Add to your platformio.ini:
```
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200

lib_deps = 
    https://github.com/gundullllla/StablePot.git

```
## Algorithm Comparison
| Algorithm        | Best For                 | Config Scope      | Default Settings                     |
|------------------|--------------------------|-------------------|--------------------------------------|
| STABLEPOT1       | High-noise environments  | Fixed preset      | αp=0.25, αs=0.7, thresh_s=0.0018    |
| STABLEPOT2       | General purpose          | Fixed preset      | αp=0.25, αs=0.7, thresh_s=0.0008    |
| STABLEPOT3       | Consistent multi-pot     | Global parameters | Customizable across all instances    |
| STABLEPOT4       | Precision applications   | Per-pot           | Fully customizable per instance     |
| EMA1             | Simple smoothing         | Fixed preset      | αp=0.25, αs=0.7                     |
| EMA2             | Tunable responsiveness   | Per-pot           | Customizable αp, αs per instance    |


## Key Configuration Parameters

### 1. Smoothing Factors (α)
void setAlphas(float primary, float secondary);
- Primary α (αp): Controls initial smoothing (0.0001-1.0)
  - Lower = more smoothing, slower response
  - Higher = less smoothing, faster response
- Secondary α (αs): Controls final output smoothing
  - Lower = ultra-stable output
  - Higher = preserves more detail
- Recommended: αp=0.1-0.3, αs=0.6-0.9

### 2. Thresholds
void setThresholds(float small, float big);
- Small Threshold: Minimum change to maintain transition state
- Big Threshold: Minimum change to trigger transition state
- Recommended ratio: Big = 1.5-2× Small

### 3. Filter Window
void setFilterTime(uint16_t time_ms);
- Transition duration in milliseconds
- Default: 122ms (optimal for most potentiometers)

### 4. Rounding Precision
void setRounding(uint8_t strength);
- Controls output decimal places (0-6)
- Higher values = more precision but more "jitter"

## Full API Reference

### Core Methods
| Method               | Description                         | Returns |
|----------------------|-------------------------------------|---------|
| update()             | Reads and processes value           | void    |
| getRawValue()        | Normalized raw ADC (0.0-1.0)        | float   |
| getSmoothedValue()   | Primary filtered output             | float   |
| getProcessedValue()  | Final processed value               | float   |
| getProcessedADC()    | Processed as ADC integer (0-4095)   | int     |

### Configuration Methods
| Method               | Description                          |
|----------------------|--------------------------------------|
| configure()          | Full configuration (STABLEPOT3/4)   |
| setAlphas()          | Set smoothing factors                |
| setThresholds()      | Set transition thresholds            |
| setFilterTime()      | Set transition duration              |
| setRounding()        | Set output precision                 |

## Example Usage

### Basic Setup (STABLEPOT1)
```
#include <StablePot.h>

StablePot volume(15, StablePot::STABLEPOT1);

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  volume.update();
  Serial.println(volume.getProcessedADC());
  delay(1);
}

### Advanced Configuration (STABLEPOT4)
#include <StablePot.h>

StablePot fineTune(32, StablePot::STABLEPOT4);

void setup() {
  fineTune.configure(
    0.15f,   // αp
    0.85f,   // αs
    0.0005f, // Small threshold
    0.0010f, // Big threshold
    75,      // Filter time (ms)
    4        // Rounding
  );
}

void loop() {
  fineTune.update();
  Serial.println(fineTune.getProcessedValue(), 4);
  delay(1);
}
```


## Troubleshooting Guide

### Problem: Output feels "jumpy"
Solution:
pot.setAlphas(0.15f, pot.getAlphaS());
pot.setThresholds(0.0010f, 0.0020f);
pot.setRounding(3);

### Problem: Slow response to changes
Solution:
pot.setFilterTime(50);
pot.setAlphas(pot.getAlphaP(), 0.85f);
float small = pot.getThresholdSmall();
pot.setThresholds(small, small * 1.5f);

### Problem: Values don't reach full range
Solution:
1. Add 0.1μF capacitor between wiper and GND
2. Verify circuit connections
3. Ensure ADC configuration:
analogReadResolution(12);
analogSetAttenuation(ADC_11db);

## Hardware Optimization Tips


### Component Selection
- Use linear-taper (B-type) potentiometers
- Add 0.1μF ceramic capacitor to wiper
- Keep analog wires short
- Ensure stable 3.3V power supply

### ESP32 ADC Setup
void setup() {
  analogReadResolution(12);       // 12-bit resolution
  analogSetAttenuation(ADC_11db); // Full 0-3.3V range
}

## Parameter Tuning Guide

### Smoothing Factors (α)
| Value | Effect                | Use Case               |
|-------|-----------------------|------------------------|
| αp=0.1 | Heavy smoothing       | Noisy environments     |
| αp=0.25 | Balanced              | General purpose        |
| αp=0.4 | Light smoothing       | Stable environments    |
| αs=0.6 | Ultra-smooth          | Critical measurements  |
| αs=0.75 | Balanced              | Most applications      |
| αs=0.9 | Responsive            | Fast controls          |

### Threshold Settings
| Value         | Effect               |
|---------------|----------------------|
| Small=0.0005  | High sensitivity     |
| Small=0.001   | Normal sensitivity   |
| Small=0.003   | Low sensitivity      |
| Big=1.5×Small | Balanced response    |
| Big=2×Small   | Less sensitive       |

### Filter Window Times
| Value  | Effect          |
|--------|-----------------|
| 50ms   | Fast response   |
| 122ms  | Balanced        |
| 250ms  | Extra smooth    |

### Rounding Precision
| Value | Effect               |
|-------|----------------------|
| 0     | Integer output       |
| 3     | 0.001 resolution     |
| 6     | Maximum precision    |


## Example Applications

### Audio Mixer Controller
```

#include <StablePot.h>

StablePot volume(15, StablePot::STABLEPOT1);
StablePot balance(32, StablePot::STABLEPOT2);
StablePot eq(33, StablePot::STABLEPOT4);

void setup() {
  eq.configure(0.1f, 0.8f, 0.0003f, 0.0006f, 150, 4);
  analogReadResolution(12);
}

void loop() {
  volume.update();
  balance.update();
  eq.update();
  // Apply to audio system
  delay(1);
}
```


### Robotics Control

```
#include <StablePot.h>

StablePot speed(15, StablePot::EMA2);
StablePot steering(32, StablePot::STABLEPOT1);

void setup() {
  speed.setAlphas(0.3f, 0.9f);
  speed.setRounding(2);
  steering.setThresholds(0.002f, 0.004f);
}

void loop() {
  speed.update();
  steering.update();
  controlRobot(speed.getProcessedADC(), steering.getProcessedADC());
  delay(1);
}
```

