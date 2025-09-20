#include <ESP32Servo.h>
#include "BluetoothSerial.h"
/**
 *      One Day Of Code
 * Arduino Car - Example code
 *       Board: Esp32
 *  Control Bluetooth Serial
 */

//#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
const char *pin = "1234"; // Change this to more secure PIN.
String device_name = "ESP32-BT-Slave";
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

// Pins to use
int PIN_SERVO_A = 18;
int PIN_SERVO_B = 19;

// Left tires
int PIN_IN1 = 27;
int PIN_IN2 = 26;

// Right tires
int PIN_IN3 = 25;
int PIN_IN4 = 33;

// Led/Light
int PIN_LED = 12;
int PIN_LED_2 = 14;

// Distance sensor HC-SR04
int PIN_ECHO = 22;
int PIN_TRIG = 23;

// Servos
Servo servoA;
Servo servoB;

//Aux
String numericPart = "";
char codeReceived;

void setup() {
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    // Init serial
    Serial.begin(115200);
    SerialBT.begin(device_name); //Bluetooth device name
    SerialBT.setTimeout(5);
    Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
#ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
#endif

    // Config pins
    pinMode(PIN_IN1, OUTPUT);
    pinMode(PIN_IN2, OUTPUT);
    pinMode(PIN_IN3, OUTPUT);
    pinMode(PIN_IN4, OUTPUT);

    // Init servo
    servoA.setPeriodHertz(50);// Standard 50hz servo
    servoA.attach(PIN_SERVO_A, 500, 2400);
    servoB.setPeriodHertz(50);// Standard 50hz servo
    servoB.attach(PIN_SERVO_B, 500, 2400);

    // Init led/light
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_LED_2, OUTPUT);

    // Init distance sensor
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

}

void loop() {

    if (Serial.available()) {
        SerialBT.write(Serial.read());
    }

    while (SerialBT.available() > 0) {
        String line = SerialBT.readStringUntil('\n');
        Serial.printf("code received: %s\n", line);
        numericPart = "";
        for (int i = 0; i < line.length(); i++) {
            int character = line[i];
            if (isDigit(character)) {
                numericPart += (char) character;
            } else if (character != '\n') {
                codeReceived = character;
            } else {
                break;
            }
        }
    }

    switch (codeReceived) {
        case 'F': goForward(); break;
        case 'L': turnLeft(); break;
        case 'B': goBack(); break;
        case 'R': turnRight(); break;
        case 'S': stop(); break;
        case 'M': turnOnLed(); break;
        case 'm': turnOffLed(); break;
        case 'N': turnOnLed2(); break;
        case 'n': turnOffLed2(); break;
        case 'J':
            if (numericPart != "") {
                rotateServoA(numericPart.toInt());
            }
            break;
        case 'K':
            if (numericPart != "") {
                rotateServoB(numericPart.toInt());
            }
            break;
    }

    SerialBT.print("D");
    SerialBT.println(measureDistance());

    delay(10);
}

long measureDistance() {
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(5);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);

    long duration = pulseIn(PIN_ECHO, HIGH);

    return microsecondsToCentimeters(duration);
}

long microsecondsToCentimeters(long microseconds) {
    return microseconds / 29 / 2;
}

void turnOnLed() {
    digitalWrite(PIN_LED, HIGH);
}

void turnOffLed() {
    digitalWrite(PIN_LED, LOW);
}

void turnOnLed2() {
    digitalWrite(PIN_LED_2, HIGH);
}

void turnOffLed2() {
    digitalWrite(PIN_LED_2, LOW);
}

void rotateServoA(int grados) {
    servoA.write(grados);
}

void rotateServoB(int grados) {
    servoB.write(grados);
}

void goForward() {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);

    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
}

void goBack() {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);

    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
}

void stop() {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);

    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, LOW);
}

void turnLeft() {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);

    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
}

void turnRight() {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);

    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
}