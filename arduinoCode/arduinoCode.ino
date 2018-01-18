/* My bluetooth rc car program */
/* The code uses an Arduino Nano board */

#include <Servo.h>
#include <SoftwareSerial.h>

const byte forwardPin = 5;
const byte backwardPin = 6;
const byte servoPin = 3;
const byte blueRX = 2;
const byte servoMin = 42;
const byte servoMax = 83;
const byte directionChange = 10;

char cmd = 0;
boolean blueConnected = false;
int direct = 0;
byte _speed = 255;

Servo servo;
SoftwareSerial blue(blueRX, 16); // 16 doesnt exist, no tx

// SECTION useful functions

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

void updateDirection() {
  turn(direct);
}

void turn(int pos) {
  servo.write(map(pos, 100, -100, servoMin, servoMax));
}

void addLeft() {
  if (direct > -100) direct -= directionChange;
}

void addRight() {
  if (direct < 100) direct += directionChange;
}

void addStraight() {
  if (direct > 0) direct -= directionChange;
  if (direct < 0) direct += directionChange;
}

void forward() {
  analogWrite(forwardPin, _speed);
  digitalWrite(backwardPin, 0);
}

void backward() {
  digitalWrite(forwardPin, 0);
  analogWrite(backwardPin, _speed);
}

void stopp() {
  digitalWrite(forwardPin, 0);
  digitalWrite(backwardPin, 0);
}

void brake() {
  digitalWrite(forwardPin, 1);
  digitalWrite(backwardPin, 1);
}

void parseSpeed(byte spedd) {
  _speed = map(spedd, 0, 100, 0, 255);
}

// SECTION program

void setup() {
  blue.begin(19200);
  pinMode(forwardPin, 1);
  pinMode(backwardPin, 1);
  servo.attach(servoPin);
  turn(0);
  stopp();

  beep();beep(); 

  Serial.begin(9600);
}

void loop() {
  
  while (blue.available()) {
    
    if (!blueConnected) {
      superBeep();
      blueConnected = true;
    }
    
    cmd = char(blue.read());

    switch(cmd) {
    case 'F':
      addStraight();
      forward();
      break;
    case 'B':
      addStraight();
      backward();
      break;
    case 'L':
      addLeft();
      break;
    case 'R':
      addRight();
      break;
    case 'G':
      addLeft();
      forward();
      break;
    case 'I':
      addRight();
      forward();
      break;
    case 'H':
      addLeft();
      backward();
      break;
    case 'J':
      addRight();
      backward();
      break;
    case 'S':
      addStraight();
      stopp();
      break;
    }
    /*
    } else if(cmd=='L') {
      addLeft();
    } else if(cmd=='R') {
      addRight();
    } else if(cmd=='G') {
      addLeft();
      forward();
    } else if(cmd=='I') {
      addRight();
      forward();
    } else if(cmd=='H') {
      addLeft();
      backward();
    } else if(cmd=='J') {
      addRight();
      backward();
    } else if(cmd=='S') {
      addStraight();
      stopp();
*/
    updateDirection();

  }
  
}


