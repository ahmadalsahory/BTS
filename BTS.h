#include "Arduino.h"
#pragma once
class Dt
{
  bool flag=true;
  double DtValue=0;
  unsigned long long Milliseconds1=0;
  unsigned long long Milliseconds2=0;
  public:
  void CalculateDt()
  {
    if(flag)
    {
      Milliseconds1=millis();
      flag=false;
      DtValue=(Milliseconds1-Milliseconds2)/1e3;
    }
    else{
      Milliseconds2=millis();
      flag=true;
      DtValue=(Milliseconds2-Milliseconds1)/1e3;
    }
  }

  void ResetTimes()
  {
    Milliseconds1= millis();
    Milliseconds2= millis();
  }

  double GetDtValue()
  {
    return DtValue;
  }
};
class BTS
{
    short RPWM,LPWM;
    double CurrentPercent=0;
    short RoundedCurrentPercent=0;
    short CurrentPwm=0;
    
    int ToPWM(short percent){
      return map(percent, -100,100,-255,255);
    }
    void Forward(short percent)
    {
      analogWrite(RPWM,ToPWM(percent));
      analogWrite(LPWM,0);
    }
    void Backward(short percent)
    {
      analogWrite(RPWM,0);
      analogWrite(LPWM,ToPWM(percent));
    }
    
public:
    Dt dt;
    void Begin(short rpwm,short lpwm)
    {
        RPWM=rpwm;
        LPWM=lpwm;
        pinMode(RPWM,OUTPUT);
        pinMode(LPWM,OUTPUT);
    }

    void Rotate(double percent)
    {
      CurrentPercent=percent;
      RoundedCurrentPercent=round(percent);
      CurrentPwm=ToPWM(RoundedCurrentPercent);

      if(percent>=0)
      {
        Forward(RoundedCurrentPercent);
      }
      else{
        Backward(RoundedCurrentPercent*-1);
      }
    }
    
    void Rotate(double percent,double acceleration)
    {
      if(percent==CurrentPercent)
      {
        return;
      }
      dt.CalculateDt();
      if(percent>CurrentPercent){
        CurrentPercent+=acceleration*dt.GetDtValue();
      }
      else{CurrentPercent-=acceleration*dt.GetDtValue();}
      Rotate(CurrentPercent);
    }

    // void RotateWithDelay(double percent,double seconds)
    // {
    //   Rotate(percent);
    //   delay(seconds*1000);
    //   dt.ResetTimes();
    // }

    // void RotateWithDelay(double percent,double seconds,double acceleration)
    // {
    //   Dt dt1;
    //   dt1.Milliseconds1=millis();
    //   dt1.Milliseconds2=millis();
    //   unsigned long long time1=millis();
    //   unsigned long long time2=time1+seconds*1000;

    //   while(time1<=time2)
    //   {
    //     dt1.CalculateDt();
    //     if(percent>CurrentPercent){
    //     CurrentPercent+=acceleration*dt1.DtValue;
    //     }
    //     else{CurrentPercent-=acceleration*dt1.DtValue;}
    //     Rotate(CurrentPercent);
    //     time1=millis();
    //     Serial.println(CurrentPercent);
    //   }
    //   dt.Milliseconds1=millis();
    //   dt.Milliseconds2=millis();
    // }

    void Stop()
    {
      analogWrite(RPWM,0);
      analogWrite(LPWM,0);
      CurrentPercent=0;
    }

    double GetCurrentPercent()
    {
      return CurrentPercent;
    }
};