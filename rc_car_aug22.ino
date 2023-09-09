
#include "BluetoothSerial.h"
BluetoothSerial ESP_BT;  //Object for Bluetooth


#define IN1 12
#define IN2 14
#define IN3 26
#define IN4 27
const String stopS = "S";
const String forwardS = "F";
const String backwardS = "B";
const String leftS = "L";
const String rightS = "R";

int incoming;

void setup() {
  Serial.begin(9600);  //Start Serial monitor in 9600
  ESP_BT.begin("Car1");
  // ledcSetup()
  Serial.println("Bluetooth Device is Ready to Pair");


  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  // digitalWrite(RF, LOW);
  // digitalWrite(LB, LOW);
  // digitalWrite(LF, LOW);
  // digitalWrite(RB, LOW);
}
void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.println("stop");
}
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("forward");
}
void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("backward");
}
void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("right");
}
void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("left");
}
void loop() {
  
  if (ESP_BT.available())  //Check if we receive anything from Bluetooth
  {
    //Read what we recevive
    incoming = ESP_BT.read();
    Serial.print("Received: ");
    Serial.println(incoming);
    // Serial.println(incoming);
    // front light on - 87, off 119
    // back light on - 85, off 117
    // horn on - 86, off 118
    // 4 indicator on - 88, off 120
    // stop
    if (incoming == 83) {
      stop();
    }

    if (incoming == 66) {
      backward();
    }
// left
    if (incoming == 70) {
      forward();
    }
// forward
    if (incoming == 76) {
      left();
    }
// left backward
    if (incoming == 82) {
      right();
    }
  }
  delay(20);
}