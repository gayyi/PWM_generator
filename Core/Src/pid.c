#include "pid.h"
#include "tim.h"
 
#define KP 3.0          // ����ϵ��
#define KI 0.01          // ����ϵ��
#define KD 0.0          // ΢��ϵ��
 
double PWM = 336.0;        //�����ź�
double PWM_out = 0.0;
double integral = 0.0;   // �������ʷ���
double derivative = 0.0; // ΢����仯����
 
double Error = 0.0;      //��ǰ���
double LastError = 0.0;  //�ϴ����
 
void PID_Control(double Now,double Set)
{
  /*PID�㷨*/
  Error = Set - Now;
  integral += Error;
  derivative = Error - LastError;
  PWM = PWM + (KP * Error + KI * integral + KD * derivative);// 
  LastError = Error;
  /*Լ��ռ�ձȵ�ֵ*/
  if(PWM > 520){
    PWM = 520;
  }else if(PWM < 150){
    PWM = 150;
  }
  /*����ռ�ձ�*/
  PWM_out = PWM;
  TIM1->CCR1 = (int) PWM_out;
  //__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,PWM);
}

void PID_Clear(void)
{
	 PWM = 336.0;        //�����ź�
	 PWM_out = 0.0;
	 integral = 0.0;   // �������ʷ���
	 derivative = 0.0; // ΢����仯����	 
	 Error = 0.0;      //��ǰ���
	 LastError = 0.0;  //�ϴ����
}
