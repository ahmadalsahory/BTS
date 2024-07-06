// ahmad hammad library
#include<BTS.h>

int PowerPercent=100; // 100% of power
BTS TestMotor;

void setup() {
  TestMotor.Begin(5,6); // in this order r_en,l_en,rpwm,lpwm
}

void loop() {
  TestMotor.Rotate(-PowerPercent,20); // (power percent,Acceleration[percent/second])
}
