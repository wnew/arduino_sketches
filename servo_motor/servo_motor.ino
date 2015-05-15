// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
int interval = 2; //

void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  myservo.write(180);
  Serial.write("Starting");
} 


void loop() {
  if (digitalRead(4) == HIGH) {
    pos = pos + interval;
    Serial.write("Button 1\n");
  }
  if (digitalRead(5) == HIGH) {
    pos = pos - interval;
    Serial.write("Button 2\n");
  }
  if (pos < 0) {
    pos = 0;
  }
  if (pos > 180) {
    pos = 180;
  }  
  myservo.write(pos);
  delay(1);
}
