#include "Arduino.h"
#pragma once
class BTS
{
    short R_EN,L_EN,RPWM,LPWM;
    double CurrentPercent=0;
    short RoundedCurrentPercent=0;
    short CurrentPwm=0;

    unsigned long long Milliseconds1=0;
    unsigned long long Milliseconds2=0;
    bool flag=true;
    double Dt=0;
    
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

    void CalculateDt()
    {
      if(flag)
      {
        Milliseconds1=millis();
        flag=false;
        Dt=(Milliseconds1-Milliseconds2)/1e3;
      }
      else{
        Milliseconds2=millis();
        flag=true;
        Dt=(Milliseconds2-Milliseconds1)/1e3;
      }
    }
    
public:
    void Begin(short r_en,short l_en,short rpwm,short lpwm)
    {
        R_EN = r_en;
        L_EN = l_en;
        RPWM=rpwm;
        LPWM=lpwm;
        pinMode(R_EN,OUTPUT);
        pinMode(L_EN,OUTPUT);
        pinMode(RPWM,OUTPUT);
        pinMode(LPWM,OUTPUT);
    }

    void Rotate(double percent)
    {
      digitalWrite(R_EN,1);
      digitalWrite(L_EN,1);

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
    

    void Rotate(short percent,double acceleration)
    {
      if(percent==RoundedCurrentPercent)
      {
        return;
      }
      CalculateDt();
      if(percent>CurrentPercent){
        CurrentPercent+=acceleration*Dt;
      }
      else{CurrentPercent-=acceleration*Dt;}
      Rotate(CurrentPercent);
    }

    void Stop()
    {
      analogWrite(RPWM,0);
      analogWrite(LPWM,0);
    }

    double GetCurrentPercent()
    {
      return CurrentPercent;
    }
};