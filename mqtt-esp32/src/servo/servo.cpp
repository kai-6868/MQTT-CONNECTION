#include <ESP32Servo.h>
#include "servo/servo.h"

static Servo myServo;
const int servoPin = 14;

void servo_init() {
  Serial.begin(115200);

  myServo.setPeriodHertz(50);        // tần số servo 50Hz
  myServo.attach(servoPin, 500, 2400); // min/max pulse (µs)

  Serial.println("ESP32 Servo Start");
}

void servo_run(uint8_t position) {

  for (int pos = 0; pos <= position ; pos++) {
    myServo.write(pos);
    delay(1);
  }

  delay(500);
}
