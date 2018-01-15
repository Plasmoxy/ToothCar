/* My bluetooth rc car program */
/* The code uses an Arduino Nano board */

#include <Servo.h>
#include <SoftwareSerial.h>

const byte forwardPin = 5;
const byte backwardPin = 6;
const byte servoPin = 3;
const byte blueRX = 2;
const byte servoMin = 42;
const byte servoMax = 85;
const byte servoStraight = (servoMin+servoMax)/2 - 3;

char cmd = 0;
boolean blueConnected = false;

Servo servo;
SoftwareSerial blue(blueRX, 16); // 16 doesnt exist, no tx

void setup() {
  blue.begin(19200);
  pinMode(forwardPin, 1);
  pinMode(backwardPin, 1);
  servo.attach(servoPin);
  straight();
  stopp();

  beep();beep(); 
}

void loop() {
  while (blue.available()) {
    
    if (!blueConnected) {
      superBeep();
      blueConnected = true;
    }
    
    cmd = char(blue.read());

    if (cmd=='F') {
      straight();
      forward();
    } else if(cmd=='B') {
      straight();
      backward();
    } else if(cmd=='L') {
      left();
    } else if(cmd=='R') {
      right();
    } else if(cmd=='G') {
      left();
      forward();
    } else if(cmd=='I') {
      right();
      forward();
    } else if(cmd=='H') {
      left();
      backward();
    } else if(cmd=='J') {
      right();
      backward();
    } else if(cmd=='S') {
      straight();
      stopp();
    }
  }
}

void beep() {
  stopp();
  analogWrite(forwardPin, 15);
  delay(80);
  digitalWrite(forwardPin, 0);
  delay(80);
}

void superBeep() {
  stopp();
  for (byte i = 0; i<10; i++) {
    analogWrite(forwardPin, 15);
    delay(30);
    analogWrite(forwardPin, 0);
    delay(30);
  }
  digitalWrite(forwardPin, 0);
}

void left() {
  servo.write(servoMax);
}

void right() {
  servo.write(servoMin);
}

void straight() {
  servo.write(servoStraight);
}

void forward() {
  digitalWrite(forwardPin, 1);
  digitalWrite(backwardPin, 0);
}

void backward() {
  digitalWrite(forwardPin, 0);
  digitalWrite(backwardPin, 1);
}

void stopp() {
  digitalWrite(forwardPin, 0);
  digitalWrite(backwardPin, 0);
}

void brake() {
  digitalWrite(forwardPin, 1);
  digitalWrite(backwardPin, 1);
}

