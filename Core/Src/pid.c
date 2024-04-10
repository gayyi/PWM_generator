#include "pid.h"
#include "tim.h"
 
#define KP 3.0          // 比例系数
#define KI 0.01          // 积分系数
#define KD 0.0          // 微分系数
 
double PWM = 336.0;        //控制信号
double PWM_out = 0.0;
double integral = 0.0;   // 积分项，历史误差
double derivative = 0.0; // 微分项，变化趋势
 
double Error = 0.0;      //当前误差
double LastError = 0.0;  //上次误差
 
void PID_Control(double Now,double Set)
{
  /*PID算法*/
  Error = Set - Now;
  integral += Error;
  derivative = Error - LastError;
  PWM = PWM + (KP * Error + KI * integral + KD * derivative);// 
  LastError = Error;
  /*约束占空比的值*/
  if(PWM > 520){
    PWM = 520;
  }else if(PWM < 150){
    PWM = 150;
  }
  /*更新占空比*/
  PWM_out = PWM;
  TIM1->CCR1 = (int) PWM_out;
  //__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,PWM);
}

void PID_Clear(void)
{
	 PWM = 336.0;        //控制信号
	 PWM_out = 0.0;
	 integral = 0.0;   // 积分项，历史误差
	 derivative = 0.0; // 微分项，变化趋势	 
	 Error = 0.0;      //当前误差
	 LastError = 0.0;  //上次误差
}
