
 
#include <Servo.h>

const byte npulse = 12; // number of pulses to charge the capacitor before each measurement
 
const byte pin_pulse = A0; // sends pulses to charge the capacitor (can be a digital pin)
const byte pin_cap  = A1; // measures the capacitor charge
const byte pin_LED = 12; // LED that turns on when metal is detected
const byte pin_buzzer = 11; // Buzzer pin - change this to your actual buzzer pin
const byte pin_servo = 8; // Servo motor pin

// Metal detection thresholds - you may need to adjust these based on your testing
const int METAL_DETECTION_THRESHOLD = 70; // Adjust this value based on your sensor sensitivity
const unsigned long DEBOUNCE_TIME = 100; // ms to prevent rapid toggling

bool metalDetected = false;
bool metalPreviouslyDetected = false; // Track previous state
unsigned long lastDetectionTime = 0;

Servo myServo; // Create servo object
 
void setup() {
  pinMode(pin_pulse, OUTPUT);
  digitalWrite(pin_pulse, LOW);
  pinMode(pin_cap, INPUT);
  pinMode(pin_LED, OUTPUT);
  digitalWrite(pin_LED, LOW);
  pinMode(pin_buzzer, OUTPUT);
  digitalWrite(pin_buzzer, LOW); // Start with buzzer off
  
  myServo.attach(pin_servo); // Attach servo to pin 8
  myServo.write(0); // Initialize servo to 0 degrees (starting position)
  
  Serial.begin(9600); // For debugging
}
 
const int nmeas = 256; //measurements to take
long int sumsum = 0; //running sum of 64 sums
long int skip = 0; //number of skipped sums
long int diff = 0;      //difference between sum and avgsum
long int flash_period = 0; //period (in ms)
long unsigned int prev_flash = 0; //time stamp of previous flash

bool servoActivated = false; // Track if servo has been activated for current detection
unsigned long servoStartTime = 0;
const unsigned long SERVO_RETURN_DELAY = 5000; // 5 seconds delay before returning to start position

void loop() {
 
  int minval = 2000;
  int maxval = 0;
 
  //perform measurement
  long unsigned int sum = 0;
  for (int imeas = 0; imeas < nmeas + 2; imeas++) {
    //reset the capacitor
    pinMode(pin_cap, OUTPUT);
    digitalWrite(pin_cap, LOW);
    delayMicroseconds(20);
    pinMode(pin_cap, INPUT);
    //apply pulses
    for (int ipulse = 0; ipulse < npulse; ipulse++) {
      digitalWrite(pin_pulse, HIGH); //takes 3.5 microseconds
      delayMicroseconds(3);
      digitalWrite(pin_pulse, LOW); //takes 3.5 microseconds
      delayMicroseconds(3);
    }
    //read the charge on the capacitor
    int val = analogRead(pin_cap); //takes 13x8=104 microseconds
    minval = min(val, minval);
    maxval = max(val, maxval);
    sum += val;
  }
 
  //subtract minimum and maximum value to remove spikes
  sum -= minval; sum -= maxval;
 
  //process
  if (sumsum == 0) sumsum = sum << 6; //set sumsum to expected value
  long int avgsum = (sumsum + 32) >> 6;
  diff = sum - avgsum;
  if (abs(diff)<avgsum >> 10) {   //adjust for small changes
    sumsum = sumsum + sum - avgsum;
    skip = 0;
  } else {
    skip++;
  }
  if (skip > 64) {  // break off in case of prolonged skipping
    sumsum = sum << 6;
    skip = 0;
  }
 
  // one permille change = 2 ticks/s
  if (diff == 0) flash_period = 1000000;
  else flash_period = avgsum / (2 * abs(diff));
  
  // Metal detection logic with buzzer control
  bool currentMetalDetection = (abs(diff) > METAL_DETECTION_THRESHOLD);
  
  // Add debouncing to prevent rapid toggling
  if (currentMetalDetection != metalDetected) {
    unsigned long currentTime = millis();
    if (currentTime - lastDetectionTime > DEBOUNCE_TIME) {
      metalDetected = currentMetalDetection;
      lastDetectionTime = currentTime;
      
      // Control buzzer and LED based on metal detection
      if (metalDetected) {
        digitalWrite(pin_buzzer, HIGH); // Turn buzzer ON
        digitalWrite(pin_LED, HIGH);    // Turn LED ON
        
        // Activate servo motor ONLY if this is a new detection (not continuous)
        if (!metalPreviouslyDetected && !servoActivated) {
          moveServoTo90();
          servoActivated = true;
          servoStartTime = currentTime;
          Serial.println("Metal detected! Servo moving to 90 degrees");
        }
      } else {
        digitalWrite(pin_buzzer, LOW);  // Turn buzzer OFF
        digitalWrite(pin_LED, LOW);     // Turn LED OFF
        // Don't reset servo immediately - let it return after 5 seconds
        Serial.println("Metal no longer detected");
      }
      
      // Update previous detection state
      metalPreviouslyDetected = metalDetected;
    }
  }

  // Handle servo return to start position after 5 seconds
  if (servoActivated) {
    unsigned long currentTime = millis();
    if (currentTime - servoStartTime >= SERVO_RETURN_DELAY) {
      returnServoToStart();
      servoActivated = false; // Reset servo activation after returning to start
      Serial.println("Servo returned to start position (0 degrees)");
    }
  }

  // For debugging - print the diff value to Serial Monitor
  // Serial.println(diff);
}

void moveServoTo90() {
  // Move servo to 90 degrees position
  myServo.write(90);
}

void returnServoToStart() {
  // Return servo to 0 degrees (starting position)
  myServo.write(0);
}