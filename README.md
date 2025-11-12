# Metal Detection System with Servo Control

An Arduino-based metal detection system that uses inductive sensing to detect metallic objects and triggers a servo motor response.

## 🌟 Features

- **Inductive Metal Detection**: Uses capacitor charging through a solenoid inductor
- **Visual & Audible Alerts**: LED and buzzer indicators for metal detection
- **Servo Motor Control**: Automated servo movement when metal is detected
- **Debounced Detection**: Prevents false triggers with time-based filtering
- **Auto-Reset**: Servo automatically returns to start position after 5 seconds

## 🛠 Hardware Components

- **Microcontroller**: Arduino Uno/Nano
- **Sensing**: Custom solenoid inductor (hand-wound copper coil)
- **Indicators**:
  - LED for visual alert
  - Buzzer for audible alert
- **Actuator**: Servo Motor
- **Passive Components**: Capacitor for charge measurement

## 📋 Pin Configuration

| Component | Arduino Pin | Notes |
|-----------|-------------|-------|
| Pulse Output | A0 | Sends pulses to charge capacitor |
| Capacitor Sense | A1 | Measures capacitor charge level |
| LED Indicator | 12 | Visual metal detection indicator |
| Buzzer | 11 | Audible metal detection alert |
| Servo Motor | 8 | Controlled actuator |

## 🔧 Circuit Setup

### Inductor Construction
**To detect metal, place the target object on the inductor**, which is made from:
- **Insulated copper wire** wound into a solenoid coil
- Multiple turns create the inductive sensing element
- The coil acts as both transmitter and receiver for electromagnetic fields

### Operating Principle
1. The system sends pulses to charge a capacitor through the inductor
2. Metal objects near the inductor change the electromagnetic field
3. This alters the capacitor charging characteristics
4. The Arduino measures these changes to detect metal presence

## 📊 System Operation

### Detection Process
1. **Normal State**: 
   - Servo at 0° position
   - LED and buzzer OFF
   - Continuous capacitor charge/discharge cycling

2. **Metal Detected**:
   - LED turns ON
   - Buzzer activates
   - Servo moves to 90° position
   - Serial monitor displays detection message

3. **Auto Reset**:
   - After 5 seconds of detection, servo returns to 0°
   - System resets for next detection cycle

### Technical Parameters
- **Pulses per measurement**: 12 pulses
- **Measurements per cycle**: 256 samples
- **Detection threshold**: 70 (adjustable)
- **Debounce time**: 100ms
- **Servo return delay**: 5000ms (5 seconds)

## 🚀 Usage Instructions

1. **Upload the code** to your Arduino board
2. **Construct the inductor** by winding insulated copper wire into a solenoid
3. **Connect all components** according to the pin configuration
4. **Power the system** and open Serial Monitor at 9600 baud
5. **Test detection** by placing metal objects on the inductor coil
6. **Adjust sensitivity** by modifying `METAL_DETECTION_THRESHOLD` if needed

## 🔧 Calibration & Tuning

### Sensitivity Adjustment
```cpp
const int METAL_DETECTION_THRESHOLD = 70; // Increase for less sensitivity, decrease for more
```

### Timing Adjustments
```cpp
const unsigned long SERVO_RETURN_DELAY = 5000; // Change servo return timing
const unsigned long DEBOUNCE_TIME = 100; // Adjust debounce period
```

## ⚠️ Important Notes

- **Inductor Quality**: Detection sensitivity depends on solenoid construction quality
- **Metal Types**: Different metals may produce varying detection strengths
- **Environmental Factors**: Electrical noise may affect detection accuracy
- **Threshold Calibration**: Test with target metals to find optimal threshold values

## 🐛 Troubleshooting

- **No Detection**: Check inductor connections and coil integrity
- **False Positives**: Increase detection threshold value
- **Servo Not Moving**: Verify servo wiring and power supply
- **Inconsistent Readings**: Ensure stable power supply and minimal electrical interference

## 📝 License

This project is open source and available under the [MIT License](LICENSE).

---

**Developed by**: ZetaEngineering INC  
**Last Updated**: December 2024
