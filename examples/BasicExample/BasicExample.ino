// ahmad hammad library
#include<BTS.h>

int PowerPercent=50; // 50% of power
BTS TestMotor;

void setup() {
  TestMotor.Begin(5,6); // in this order rpwm,lpwm
}
void loop() {
  TestMotor.Rotate(PowerPercent); // this line make motor work with 50% of power
  delay(3000);
  TestMotor.Stop();
  delay(3000);
  TestMotor.Rotate(-PowerPercent); // this line make motor work with 50% of power but In the opposite direction (الاتجاه المعاكس)
  delay(3000);
}