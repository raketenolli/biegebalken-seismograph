#include <MsTimer2.h>
#include <Servo.h>

Servo myservo;

int pos = 0;

int led_gr = 9;
int led_ge = 8;
int led_ro = 7;

void rot_blinken() {
  static boolean output = HIGH;
  digitalWrite(led_ro, output);
  output = !output;
}

void setup() {
  pinMode(led_gr, OUTPUT);
  pinMode(led_ge, OUTPUT);
  pinMode(led_ro, OUTPUT);

  myservo.attach(6);

  MsTimer2::set(100, rot_blinken);
}

void loop() {
  digitalWrite(led_gr, 1);
  delay(3000);
  digitalWrite(led_gr, 0);

  digitalWrite(led_ge, 1);
  delay(3000);
  digitalWrite(led_ge, 0);
  
  MsTimer2::start();
  for(pos = 30; pos <= 150; pos += 30)
  {
    myservo.write(pos);
    Serial.println(pos);
    delay(500);
  } 
  MsTimer2::stop();

  digitalWrite(led_ro, 0);
}

