#include <MsTimer2.h>
#include "HX711.h"
#include <Servo.h>

#define DOUT 2
#define CLK 3

#define LED_GR 9
#define LED_GE 8
#define LED_RO 7

#define SERVO_PIN 6

HX711 scale(DOUT, CLK);
Servo myservo;

float calibration_factor = -2034; //-2034 für den Ebay-1 kg-Biegebalken. Ggf. mit echtem Kalibriergewicht nachmessen
int pos = 0;
float f;

void rot_blinken() {
  static boolean output = HIGH;
  digitalWrite(LED_RO, output);
  output = !output;
}

void LED_status(int gr, int ge, int ro) {
  static boolean rot_blinkt = LOW;
  digitalWrite(LED_GR, gr);
  digitalWrite(LED_GE, ge);
  if (ro == 1 && !rot_blinkt) {
    rot_blinkt = HIGH;
    MsTimer2::start();
  }
  if (ro == 0 && rot_blinkt) {
    rot_blinkt = LOW;
    MsTimer2::stop();
    digitalWrite(LED_RO,ro);
  }
}

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_GR, OUTPUT);
  pinMode(LED_GE, OUTPUT);
  pinMode(LED_RO, OUTPUT);

  myservo.attach(SERVO_PIN);

  scale.set_scale();
  scale.tare();  //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading
  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  MsTimer2::set(100, rot_blinken);
}

void loop() {
  f = scale.get_units();
  Serial.println(f, 2);
  delay(25);

  if (f <= 10.0) {
    LED_status(1,0,0);
  }
  else if (f < 500.0) {
    LED_status(0,1,0);
  }
  else {
    LED_status(0,1,1);
  }

  pos = int(constrain(map(f, 0, 1000, 0, 180), 0, 180));
  myservo.write(pos);

}

