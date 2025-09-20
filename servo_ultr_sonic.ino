#include <ESP32Servo.h>

// declare all pins
const int servo_pin = 22;
const int trig_pin = 14;
const int echo_pin = 12;

Servo servo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  servo.attach(servo_pin);
  servo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(trig_pin, HIGH);
  long duration = pulseIn(echo_pin, 1);
  long  distance = (duration / 2) * 0.0343;
  int angle = map(distance, 5, 400, 0, 180);
  servo.write(angle);
  Serial.println("angle: ");
  Serial.println(angle);

  delay(100);
}
